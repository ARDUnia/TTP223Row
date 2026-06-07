/*
  Example: Media Player Controller
  by: Hamidreza Milaninia [ARDUnia]
  Library: TTP223Row
  Description: Use a 4-sensor strip to control media playback.

    سناریو: فرض کنید ۴ سنسور را زیر لبه‌ی میز کار یا روی فرمان ماشین نصب کرده‌اید تا با آن موسیقی کامپیوتر یا یک ماژول MP3 را کنترل کنید. در این مثال از قابلیت‌های «دو ضرب» و «لمس چند انگشتی» استفاده می‌کنیم.
    کشیدن دست به راست (L2R): آهنگ بعدی (Next Track).
    کشیدن دست به چپ (R2L): آهنگ قبلی (Prev Track).
    دو ضرب روی سنسور اول (2TS1): پخش / توقف (Play / Pause).
    لمس همزمان با دو انگشت (1T2F): قطع صدا (Mute).
    لمس رفت و برگشتی (RLLR): تغییر سورس (مثلاً از بلوتوث به رادیو).

*/

#include "TTP223Row.h"

// تعریف 4 سنسور متصل به پین های 2، 3، 4 و 5
TTP223Row mediaStrip(4, 2, 3, 4, 5);

void setup() {
  Serial.begin(115200);
  mediaStrip.begin();
  Serial.println("Media Controller Initialized.");
  Serial.println("Waiting for gestures...");
}

void loop() {
  String gesture = mediaStrip.HowTouch();
  
  if (gesture != "") {
    
    if (gesture == "L2R") {
      // در یک پروژه واقعی، اینجا دستور Next را به ماژول بلوتوث یا PC می‌فرستید
      Serial.println("Command: NEXT TRACK >>|");
    } 
    else if (gesture == "R2L") {
      Serial.println("Command: PREV TRACK |<<");
    } 
    else if (gesture == "2TS1") {
      // دو ضرب روی سنسور اول
      Serial.println("Command: PLAY / PAUSE >||");
    } 
    else if (gesture == "1T2F") {
      // تک ضرب اما با دو انگشت همزمان (روی هر کدام از سنسورها که باشد)
      Serial.println("Command: MUTE / UNMUTE (X)");
    }
    else if (gesture == "RLLR") {
      // حرکت رفت و برگشتی دست روی سنسورها
      Serial.println("Command: CHANGE SOURCE (Bluetooth -> FM Radio)");
    }
    else if (gesture == "LT2F") {
      // لمس طولانی همزمان با دو انگشت
      Serial.println("Command: SYSTEM POWER OFF");
    }
    else {
      // برای مشاهده سایر جسچرهای تشخیص داده شده که در دستورات بالا نیستند
      Serial.print("Other gesture detected: ");
      Serial.println(gesture);
    }
  }
}
