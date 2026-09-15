# TTP223Row Arduino Library by Hamidreza Milaninia [ARDUnia]
[Hamidreza Milaninia – Personal Website](https://hrmnia.com/)

A robust, non-blocking Arduino library designed to process complex gestures from a row of 2 to 5 inline TTP223 capacitive touch sensors.

## Key Advantages
* **Zero Dependencies:** Completely standalone. It relies solely on the standard `Arduino.h` core, meaning you don't need to install any other libraries.
* **Lightweight & Fast:** Highly optimized using a state-machine with a minimal memory footprint, making it perfect for resource-constrained microcontrollers.
* **Cross-Platform Compatibility:** Works seamlessly across various boards including AVR (Uno, Mega, Nano), ESP8266, ESP32, and other Arduino-compatible MCUs.
* **Plug & Play:** Extremely easy to install and integrate into your projects without the headache of managing prerequisite libraries.

## Features
* **Multi-Sensor Support:** Easily configure between 2 to 5 sensors in a row.
* **Non-Blocking Architecture:** Uses `millis()` for timing, ensuring your main loop runs smoothly without interruptions.
* **Tap Detection:** Recognizes single and multi-taps (e.g., 2TS, 3TS).
* **Multi-Finger Detection:** Identifies simultaneous multi-finger touches and taps (e.g., 2T2F, 3T3F).
* **Swipe Gestures:** Detects directional swipes including Left-to-Right (L2R), Right-to-Left (R2L), and bidirectional sweeps (RLLR).
* **Long Press Detection:** Recognizes single or multi-finger long touches (e.g., LT1F, LT2F).

## Installation
1. Download this repository as a `.zip` file.
2. Open the Arduino IDE.
3. Go to **Sketch** -> **Include Library** -> **Add .ZIP Library...**
4. Select the downloaded `.zip` file.

## Quick Start

```cpp
#include "TTP223Row.h"

// Initialize 4 sensors connected to pins 2, 3, 4, and 5
TTP223Row mySensors(4, 2, 3, 4, 5); 

void setup() {
  Serial.begin(115200);
  mySensors.begin();
}

void loop() {
  String gesture = mySensors.HowTouch();
  
  if (gesture != "") {
    Serial.print("Gesture: ");
    Serial.println(gesture);
  }
}


Supported Gesture Outputs

    1TS1, 1TS2: Single short touch on sensor 1, 2, etc.

    2TS1, 3TS2: Double or triple short tap on a specific sensor.

    1T2F, 2T3F: Single tap with 2 fingers, double tap with 3 fingers, etc.

    L2R: Swipe Left to Right.

    R2L: Swipe Right to Left.

    RLLR: Swipe back and forth.

    LT1F_1, LT2F: Long touch with 1 finger (on a specific sensor) or 2 fingers.

License

This project is licensed under the MIT License - see the LICENSE file for details.


