/*
TTP223Row.h BasicUsage Example
by Hamidreza Milaninia [ARDUnia]
*/

#include "TTP223Row.h"

// ایجاد نمونه از کلاس: 4 سنسور متصل به پین های 2، 3، 4 و 5
TTP223Row mySensors(4, 2, 3, 4, 5); 

void setup() {
  Serial.begin(9600);
  mySensors.begin(); // راه اندازی پین ها
  Serial.println("System Ready. Waiting for touch...");
}

void loop() {
  // دریافت وضعیت سنسورها به صورت غیر مسدود کننده
  String gesture = mySensors.HowTouch();
  
  if (gesture != "") {
    Serial.print("Detected Gesture: ");
    Serial.println(gesture);
    
    // اعمال واکنش ها بر اساس خروجی
    if (gesture == "1TS1") {
      // کد مربوط به لمس کوتاه تک ضرب روی سنسور 1
    } 
    else if (gesture == "L2R") {
      // کد مربوط به کشیدن دست از چپ به راست
    }
    else if (gesture == "2T2F") {
      // لمس دو ضرب با دو انگشت همزمان
    }
  }
}