# Changelog

All notable changes to TTP223Row are documented in this file.

## [1.1.0] - 2026-09-19

### Fixed

- Correctly reports multi-finger long presses, including staggered releases.
- Distinguishes simultaneous multi-finger taps from sequential sensor taps.
- Removes the unclassified 500–999 ms touch-duration gap.
- Rejects invalid sensor counts, missing pins, and duplicate pins.
- Requires exact swipe sequences within a configurable duration window.
- Prevents simultaneous touches from being mistaken for directional swipes.

### Added

- `readGesture()` as the preferred alias for `HowTouch()`.
- `isValid()` and a Boolean return value from `begin()`.
- `setTiming()` for runtime timing configuration.
- Fixed-size internal sequence tracking.
- Host-side gesture regression tests.
- CI compilation for Arduino Uno, ESP8266, and ESP32.
- PlatformIO `library.json` metadata.

### Documentation

- Rebuilt the English and Persian READMEs with complete Markdown formatting.
- Added Library Manager, PlatformIO, and ZIP installation instructions.
- Added gesture, timing, validation, and development references.

## [1.0.4] - 2026-06-07

- Initial public release.

[1.1.0]: https://github.com/ARDUnia/TTP223Row/compare/1.0.4...HEAD
[1.0.4]: https://github.com/ARDUnia/TTP223Row/releases/tag/1.0.4
