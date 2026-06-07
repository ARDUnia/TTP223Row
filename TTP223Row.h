/*
TTP223Row.h by Hamidreza Milaninia [ARDUnia]
ver : 1.0.4
*/

#ifndef TTP223Row_h
#define TTP223Row_h

#include "Arduino.h"

class TTP223Row {
  private:
    uint8_t _pins[5];
    uint8_t _numSensors;
    
    // متغیرهای وضعیت سنسورها
    unsigned long _touchStartTime[5];
    int _tapCount[5];
    bool _isTouched[5];
    bool _wasTouched[5];
    
    // متغیرهای ردیابی دقیق توالی حرکت (Swipe Tracking)
    String _onSequence;
    int _maxActiveTouches;
    unsigned long _globalGestureTimer;
    
    // تنظیمات زمانی بر حسب میلی‌ثانیه (قابل تنظیم)
    const unsigned long SHORT_TOUCH_MAX = 500;  // حداکثر زمان برای یک ضربه معمولی
    const unsigned long LONG_TOUCH_MIN = 1000;  // حداقل زمان برای لمس طولانی
    const unsigned long MULTI_TAP_GAP = 400;     // مهلت زمانی برای اتمام کل جسچر

    void resetStates();

  public:
    // سازنده کلاس با قابلیت پشتیبانی از ۲ تا ۵ سنسور
    TTP223Row(uint8_t num, uint8_t p1, uint8_t p2, int p3 = -1, int p4 = -1, int p5 = -1);
    void begin();
    String HowTouch();
};

#endif
