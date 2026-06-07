/*
TTP223Row.cpp by Hamidreza Milaninia [ARDUnia]
ver : 1.0.4
*/

#include "TTP223Row.h"

// سازنده کلاس و مقداردهی اولیه پین‌ها
TTP223Row::TTP223Row(uint8_t num, uint8_t p1, uint8_t p2, int p3, int p4, int p5) {
  _numSensors = constrain(num, 2, 5);
  _pins[0] = p1;
  _pins[1] = p2;
  if (_numSensors > 2) _pins[2] = p3;
  if (_numSensors > 3) _pins[3] = p4;
  if (_numSensors > 4) _pins[4] = p5;
}

// کانفیگ پین‌ها و ریست کردن آرایه‌ها در شروع برنامه
void TTP223Row::begin() {
  for (int i = 0; i < _numSensors; i++) {
    pinMode(_pins[i], INPUT);
    _isTouched[i] = false;
    _wasTouched[i] = false;
    _tapCount[i] = 0;
    _touchStartTime[i] = 0;
  }
  _onSequence = "";
  _maxActiveTouches = 0;
  _globalGestureTimer = 0;
}

// ریست کردن شمارنده‌ها برای شروع یک جسچر جدید
void TTP223Row::resetStates() {
  for (int i = 0; i < _numSensors; i++) {
    _tapCount[i] = 0;
  }
  _onSequence = "";
  _maxActiveTouches = 0;
}

// تابع اصلی پردازش و تشخیص نوع لمس
String TTP223Row::HowTouch() {
  unsigned long currentMillis = millis();
  int activeTouches = 0;
  String result = "";

  // ۱. بررسی وضعیت فعلی سنسورها (کدام سنسورها در این لحظه High هستند)
  for (int i = 0; i < _numSensors; i++) {
    _isTouched[i] = digitalRead(_pins[i]) == HIGH;
    if (_isTouched[i]) activeTouches++;
  }

  // ثبت بیشترین تعداد انگشتی که در یک لحظه همزمان روی سنسورها قرار گرفته
  if (activeTouches > _maxActiveTouches) {
    _maxActiveTouches = activeTouches;
  }

  // ۲. پردازش لبه‌های بالا رونده و پایین رونده سیگنال سنسورها
  for (int i = 0; i < _numSensors; i++) {
    // الف) سنسور تازه لمس شده است (لبه بالا رونده)
    if (_isTouched[i] && !_wasTouched[i]) {
      _touchStartTime[i] = currentMillis;
      _globalGestureTimer = currentMillis;
      
      // اضافه کردن شماره سنسور به رشته توالی (جلوگیری از ثبت متوالی یک سنسور)
      char pinChar = '1' + i;
      if (_onSequence.length() == 0 || _onSequence.charAt(_onSequence.length() - 1) != pinChar) {
        _onSequence += pinChar;
      }
    }
    
    // ب) دست از روی سنسور برداشته شده است (لبه پایین رونده)
    if (!_isTouched[i] && _wasTouched[i]) {
      unsigned long touchDuration = currentMillis - _touchStartTime[i];
      
      if (touchDuration < SHORT_TOUCH_MAX) {
        _tapCount[i]++; // ثبت به عنوان کاندیدای کلیک/ضربه
      } else if (touchDuration >= LONG_TOUCH_MIN) {
        // ثبت لمس طولانی (اگر همه سنسورها رها شده باشند، موقعیت سنسور را هم اعلام می‌کند)
        if (activeTouches == 0) result = "LT1F_" + String(i + 1);
        else result = "LT" + String(activeTouches + 1) + "F";
      }
      _globalGestureTimer = currentMillis; // تمدید مهلت زمانی زمان‌سنج عمومی
    }
    _wasTouched[i] = _isTouched[i];
  }

  // اگر لمس طولانی اتفاق افتاده و دست کاملاً برداشته شده، بلافاصله نتیجه را برگردان
  if (result.startsWith("LT") && activeTouches == 0) {
    resetStates();
    return result;
  }

  // ۳. ارزیابی نهایی (وقتی دست کاملاً از روی سنسورها برداشته شده و زمان مکث نهایی به پایان رسیده)
  if (activeTouches == 0 && _onSequence.length() > 0 && (currentMillis - _globalGestureTimer > MULTI_TAP_GAP)) {
    
    // =================================================================
    // اولویت اول و اصلی: بررسی حرکت لمس حرکتی (Swipe) با اولویت مطلق
    // =================================================================
    bool isL2R = false;
    bool isR2L = false;

    // بررسی توالی چپ به راست (مثلاً بررسی وجود الگوی ۱ و بعد ۲ و بعد ۳ در رشته توالی)
    int targetL2R = 1;
    for(unsigned int i = 0; i < _onSequence.length(); i++) {
      if(_onSequence.charAt(i) == '0' + targetL2R) targetL2R++;
    }
    if (targetL2R > _numSensors) isL2R = true;

    // بررسی توالی راست به چپ
    int targetR2L = _numSensors;
    for(unsigned int i = 0; i < _onSequence.length(); i++) {
      if(_onSequence.charAt(i) == '0' + targetR2L) targetR2L--;
    }
    if (targetR2L < 1) isR2L = true;

    // فیلتر هوشمند تفاوت زمانی برای تفکیک سوایپ سریع از لمس همزمان چند انگشت (1T2F)
    unsigned long timeDiff = 0;
    if (_touchStartTime[0] > _touchStartTime[_numSensors - 1]) {
      timeDiff = _touchStartTime[0] - _touchStartTime[_numSensors - 1];
    } else {
      timeDiff = _touchStartTime[_numSensors - 1] - _touchStartTime[0];
    }

    // اگر توالی حرکتی درست است و سنسور اول و آخر با یک تاخیر منطقی (بیشتر از 60ms) فعال شده‌اند
    if (timeDiff > 60) {
      if (isL2R && isR2L) result = "RLLR"; // حرکت رفت و برگشتی سریع
      else if (isL2R) result = "L2R";       // حرکت چپ به راست
      else if (isR2L) result = "R2L";       // حرکت راست به چپ
    }

    // =================================================================
    // اولویت دوم: اگر الگو سوایپ نبود، ضربه‌ها (Taps) و تعداد انگشت را حساب کن
    // =================================================================
    if (result == "") {
      int multiFingerCount = 0;
      int maxTaps = 0;
      int lastTappedPin = -1;

      for (int i = 0; i < _numSensors; i++) {
        if (_tapCount[i] > 0) {
          multiFingerCount++;
          lastTappedPin = i + 1;
          if (_tapCount[i] > maxTaps) maxTaps = _tapCount[i];
        }
      }

      if (multiFingerCount == 1) {
        // ضربه تک انگشتی روی یک سنسور خاص (مثلاً 1TS2 یعنی یک ضربه روی سنسور ۲)
        result = String(maxTaps) + "TS" + String(lastTappedPin);
      } else if (multiFingerCount > 1) {
        // ضربه چند انگشتی همزمان (مثلاً 1T2F یعنی یک ضربه همزمان با دو انگشت)
        result = String(maxTaps) + "T" + String(multiFingerCount) + "F";
      }
    }

    // ریست کردن تمام متغیرها برای آمادگی جهت جسچر بعدی
    resetStates();
  }

  return result;
}
