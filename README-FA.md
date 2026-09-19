# کتابخانه آردوینو TTP223Row

[![CI](https://github.com/ARDUnia/TTP223Row/actions/workflows/ci.yml/badge.svg)](https://github.com/ARDUnia/TTP223Row/actions/workflows/ci.yml)
[![Version](https://img.shields.io/badge/version-1.1.0-blue.svg)](https://github.com/ARDUnia/TTP223Row/releases)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

کتابخانه ای غیرمسدودکننده برای تشخیص ضربه، لمس همزمان چند انگشتی، لمس طولانی و حرکت های جهت دار روی یک ردیف ۲ تا ۵ عددی از سنسورهای لمسی خازنی TTP223.

مستندات انگلیسی: [README.md](README.md)  
سازنده: [حمیدرضا میلانی نیا](https://hrmnia.com/) — [ARDUnia](https://ardunia.ir/)

## ویژگی های اصلی

- پشتیبانی از ۲ تا ۵ سنسور خطی
- اجرای کاملا غیرمسدودکننده با `millis()`
- تفکیک لمس همزمان چند انگشتی از لمس متوالی چند سنسور
- تشخیص دقیق حرکت چپ به راست، راست به چپ و رفت و برگشت
- استفاده از بافرهای داخلی با اندازه ثابت برای پایداری بیشتر روی AVR
- حفظ تابع قدیمی `HowTouch()` و اضافه شدن تابع خواناتر `readGesture()`
- بدون نیاز به کتابخانه جانبی

## نصب

### Arduino Library Manager

در Arduino IDE وارد **Tools > Manage Libraries** شوید، عبارت **TTP223Row** را جستجو کنید و **Install** را بزنید.

### PlatformIO

کتابخانه را از Registry نصب کنید یا خط زیر را به `platformio.ini` اضافه کنید:

```ini
lib_deps = ARDUnia/TTP223Row
```

### نصب فایل ZIP

مخزن را به صورت ZIP دانلود کنید و در Arduino IDE از مسیر **Sketch > Include Library > Add .ZIP Library** آن را نصب کنید.

## اتصال سنسورها

خروجی سیگنال هر TTP223 را به یک ورودی دیجیتال جداگانه وصل کنید. ترتیب پین ها در سازنده باید با ترتیب فیزیکی سنسورها از چپ به راست یکسان باشد. کتابخانه برای حالت استاندارد active-HIGH سنسور TTP223 طراحی شده است.

## راه اندازی سریع

```cpp
#include <TTP223Row.h>

TTP223Row touchRow(4, 2, 3, 4, 5);

void setup() {
  Serial.begin(115200);
  if (!touchRow.begin()) {
    Serial.println("Invalid TTP223Row configuration");
  }
}

void loop() {
  String gesture = touchRow.readGesture();
  if (gesture != "") {
    Serial.println(gesture);
  }
}
```

تابع `HowTouch()` برای سازگاری با برنامه های قبلی حفظ شده و همان کار `readGesture()` را انجام می دهد.

## خروجی حرکت ها

| خروجی | معنی |
| --- | --- |
| `1TS1` | یک ضربه کوتاه روی سنسور ۱ |
| `2TS2` | دو ضربه کوتاه روی سنسور ۲ |
| `1T2F` | یک ضربه همزمان با دو انگشت |
| `2T3F` | دو ضربه هماهنگ و همزمان با سه انگشت |
| `L2R` | حرکت دقیق از چپ به راست روی تمام ردیف |
| `R2L` | حرکت دقیق از راست به چپ روی تمام ردیف |
| `RLLR` | حرکت کامل چپ به راست و بازگشت به چپ |
| `LT1F_2` | لمس طولانی سنسور ۲ |
| `LT2F` | لمس طولانی با دو انگشت که هر دو از آستانه عبور کرده اند |

الگوهای ترکیبی یا مبهم عمدا نادیده گرفته می شوند تا به اشتباه لمس چند انگشتی گزارش نشوند.

## تنظیم زمان ها

مقادیر پیش فرض:

| تنظیم | مقدار پیش فرض |
| --- | ---: |
| آستانه لمس طولانی | ۱۰۰۰ میلی ثانیه |
| مهلت پایان چند ضربه | ۴۰۰ میلی ثانیه |
| حداقل زمان Swipe | ۶۰ میلی ثانیه |
| حداکثر زمان Swipe | ۱۵۰۰ میلی ثانیه |

تمام لمس های کوتاه تر از آستانه لمس طولانی، Tap محسوب می شوند و دیگر فاصله زمانی تعریف نشده وجود ندارد. برای تغییر زمان ها از تابع زیر استفاده کنید:

```cpp
bool accepted = touchRow.setTiming(
  800,   // longPressMin
  350,   // multiTapGap
  50,    // swipeMinDuration
  1200   // swipeMaxDuration
);
```

مقادیر نامعتبر پذیرفته نمی شوند و تابع `false` برمی گرداند.

## اعتبارسنجی تنظیمات

تعداد سنسور خارج از بازه ۲ تا ۵، پین حذف شده و پین تکراری رد می شوند. برای تشخیص تنظیمات نامعتبر می توانید از `isValid()` یا مقدار برگشتی `begin()` استفاده کنید.

## مثال ها

- `BasicUsage`: اعتبارسنجی و خواندن پایه حرکت ها
- `MediaPlayerController`: کنترل پخش با Swipe و Tap
- `SmartHomeLighting`: کنترل رله یا LED با پنل سه سنسوری

## توسعه و آزمایش

گردش کار CI تست های منطقی حرکت ها را اجرا می کند و تمام مثال ها را برای Arduino Uno، ESP8266 و ESP32 کامپایل می کند. روند مشارکت در [CONTRIBUTING.md](CONTRIBUTING.md) و تاریخچه تغییرات در [CHANGELOG.md](CHANGELOG.md) آمده است.

## مجوز

این پروژه تحت [مجوز MIT](LICENSE) منتشر شده است.
