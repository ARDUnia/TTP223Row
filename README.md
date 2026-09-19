# TTP223Row Arduino Library

[![CI](https://github.com/ARDUnia/TTP223Row/actions/workflows/ci.yml/badge.svg)](https://github.com/ARDUnia/TTP223Row/actions/workflows/ci.yml)
[![Version](https://img.shields.io/badge/version-1.1.0-blue.svg)](https://github.com/ARDUnia/TTP223Row/releases)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A non-blocking Arduino library for recognizing taps, multi-finger touches, long presses, and directional swipes across a linear row of 2 to 5 TTP223 capacitive touch sensors.

Persian documentation: [README-FA.md](README-FA.md)  
Author: [Hamidreza Milaninia](https://hrmnia.com/) — [ARDUnia](https://ardunia.ir/)

## Highlights

- Supports 2 to 5 inline sensors.
- Uses `millis()` and never blocks the main loop.
- Distinguishes simultaneous multi-finger touches from sequential taps.
- Detects exact left-to-right, right-to-left, and round-trip swipe sequences.
- Uses fixed-size internal buffers to remain predictable on AVR boards.
- Preserves the original `HowTouch()` API and adds the clearer `readGesture()` alias.
- Has no third-party dependencies.

## Installation

### Arduino Library Manager

Open **Tools > Manage Libraries**, search for **TTP223Row**, and select **Install**.

### PlatformIO

Add the library from the PlatformIO Registry or add this line to `platformio.ini`:

```ini
lib_deps = ARDUnia/TTP223Row
```

### ZIP installation

Download this repository as a ZIP file, then select **Sketch > Include Library > Add .ZIP Library** in the Arduino IDE.

## Wiring

Connect each TTP223 signal output to a separate digital input. The constructor arguments must follow the physical left-to-right order of the sensors. The library expects the standard active-HIGH TTP223 output mode.

## Quick start

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

`HowTouch()` remains available as a backward-compatible alias for `readGesture()`.

## Gesture outputs

| Output | Meaning |
| --- | --- |
| `1TS1` | One short tap on sensor 1 |
| `2TS2` | Two short taps on sensor 2 |
| `1T2F` | One simultaneous two-finger tap |
| `2T3F` | Two consistent, simultaneous three-finger taps |
| `L2R` | Exact left-to-right swipe across the row |
| `R2L` | Exact right-to-left swipe across the row |
| `RLLR` | Exact left-to-right-to-left round trip |
| `LT1F_2` | Long press on sensor 2 |
| `LT2F` | Long press by two fingers, each held past the threshold |

Mixed or ambiguous tap patterns are intentionally ignored rather than being reported as false multi-finger gestures.

## Timing configuration

Default values are:

| Setting | Default |
| --- | ---: |
| Long-press threshold | 1000 ms |
| Multi-tap completion gap | 400 ms |
| Minimum swipe duration | 60 ms |
| Maximum swipe duration | 1500 ms |

All touches shorter than the long-press threshold are eligible taps, so there is no unclassified timing gap. Configure the values before or after `begin()`:

```cpp
bool accepted = touchRow.setTiming(
  800,   // longPressMin
  350,   // multiTapGap
  50,    // swipeMinDuration
  1200   // swipeMaxDuration
);
```

Invalid timing values are rejected and return `false`.

## Configuration validation

The constructor rejects sensor counts outside 2 to 5, missing required pins, and duplicate pins. Use `isValid()` or the return value of `begin()` to detect invalid configuration.

## Examples

- `BasicUsage`: validation and basic gesture reading
- `MediaPlayerController`: media actions with swipe and tap gestures
- `SmartHomeLighting`: relay or LED control using a three-sensor panel

## Development

The CI workflow runs deterministic host-side gesture tests and compiles every example for Arduino Uno, ESP8266, and ESP32. See [CONTRIBUTING.md](CONTRIBUTING.md) for the contribution workflow and [CHANGELOG.md](CHANGELOG.md) for release history.

## License

Released under the [MIT License](LICENSE).
