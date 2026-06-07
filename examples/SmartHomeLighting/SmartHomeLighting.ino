/*
  Example: Smart Home Lighting Control
  by: Hamidreza Milaninia [ARDUnia]
  Library: TTP223Row
  Description: Control 3 relays/LEDs using tap and swipe gestures.

    سناریو: فرض کنید ۳ عدد از این سنسورها را پشت یک پنل شیشه‌ای روی دیوار نصب کرده‌اید و می‌خواهید ۳ لامپ یا رله را کنترل کنید. علاوه بر روشن/خاموش کردن تکی، می‌خواهید با حرکت دست (Swipe) همه لامپ‌ها را یکجا روشن یا خاموش کنید.
    تک ضرب روی هر سنسور: تغییر وضعیت (Toggle) همان لامپ.
    کشیدن دست از چپ به راست (L2R): روشن شدن تمام لامپ‌ها.
    کشیدن دست از راست به چپ (R2L): خاموش شدن تمام لامپ‌ها.
    لمس طولانی سنسور وسط (LT1F_2): فعال‌سازی حالت خواب (خاموش شدن همه به جز یک لامپ خواب).

*/

#include "TTP223Row.h"

// تعریف 3 سنسور متصل به پین های 2، 3 و 4
TTP223Row wallPanel(3, 2, 3, 4);

// تعریف پین های خروجی برای رله ها یا LED ها
const int RELAY_1 = 8;
const int RELAY_2 = 9;
const int RELAY_3 = 10;

// متغیرهایی برای ذخیره وضعیت فعلی هر لامپ
bool state1 = false;
bool state2 = false;
bool state3 = false;

void setup() {
  Serial.begin(115200);
  wallPanel.begin();
  
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  
  Serial.println("Smart Home Panel Ready!");
}

void loop() {
  String gesture = wallPanel.HowTouch();
  
  if (gesture != "") {
    Serial.println("Action: " + gesture);
    
    // کنترل تکی لامپ ها با تک ضرب
    if (gesture == "1TS1") {
      state1 = !state1;
      digitalWrite(RELAY_1, state1 ? HIGH : LOW);
    } 
    else if (gesture == "1TS2") {
      state2 = !state2;
      digitalWrite(RELAY_2, state2 ? HIGH : LOW);
    } 
    else if (gesture == "1TS3") {
      state3 = !state3;
      digitalWrite(RELAY_3, state3 ? HIGH : LOW);
    }
    
    // روشن کردن همه با کشیدن دست به راست
    else if (gesture == "L2R") {
      state1 = state2 = state3 = true;
      digitalWrite(RELAY_1, HIGH);
      digitalWrite(RELAY_2, HIGH);
      digitalWrite(RELAY_3, HIGH);
      Serial.println("ALL LIGHTS ON");
    }
    
    // خاموش کردن همه با کشیدن دست به چپ
    else if (gesture == "R2L") {
      state1 = state2 = state3 = false;
      digitalWrite(RELAY_1, LOW);
      digitalWrite(RELAY_2, LOW);
      digitalWrite(RELAY_3, LOW);
      Serial.println("ALL LIGHTS OFF");
    }
    
    // حالت خواب با لمس طولانی سنسور وسط (سنسور 2)
    else if (gesture == "LT1F_2") {
      state1 = state3 = false;
      state2 = true; // فقط لامپ وسط (مثلا لامپ خواب) روشن بماند
      digitalWrite(RELAY_1, LOW);
      digitalWrite(RELAY_2, HIGH);
      digitalWrite(RELAY_3, LOW);
      Serial.println("NIGHT MODE ACTIVATED");
    }
  }
}