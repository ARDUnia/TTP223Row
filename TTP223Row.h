/*
  TTP223Row.h
  Copyright (c) 2026 Hamidreza Milaninia [ARDUnia]
  Version 1.1.0
*/

#ifndef TTP223Row_h
#define TTP223Row_h

#include "Arduino.h"

class TTP223Row {
 public:
  TTP223Row(uint8_t num, int p1, int p2, int p3 = -1, int p4 = -1,
            int p5 = -1);

  // Returns false when the sensor count or one of the required pins is invalid.
  bool begin();
  bool isValid() const;

  // All values are milliseconds. Returns false for an invalid configuration.
  bool setTiming(unsigned long longPressMin, unsigned long multiTapGap,
                 unsigned long swipeMinDuration,
                 unsigned long swipeMaxDuration);

  // Preferred API name.
  String readGesture();

  // Backward-compatible alias retained for existing sketches.
  String HowTouch();

 private:
  static const uint8_t MAX_SENSORS = 5;
  static const uint8_t MAX_SEQUENCE = 16;

  uint8_t _pins[MAX_SENSORS];
  uint8_t _numSensors;
  bool _valid;
  bool _begun;

  unsigned long _touchStartTime[MAX_SENSORS];
  bool _isTouched[MAX_SENSORS];
  bool _wasTouched[MAX_SENSORS];

  uint8_t _sequence[MAX_SEQUENCE];
  unsigned long _sequenceTime[MAX_SEQUENCE];
  uint8_t _sequenceLength;
  bool _sequenceOverflow;

  uint8_t _previousActiveTouches;
  uint8_t _currentGroupMask;
  uint8_t _currentGroupMaxActive;
  uint8_t _tapPatternMask;
  uint8_t _tapGroupCount;
  bool _tapPatternConsistent;
  uint8_t _longPressMask;
  unsigned long _globalGestureTimer;

  unsigned long _longPressMin;
  unsigned long _multiTapGap;
  unsigned long _swipeMinDuration;
  unsigned long _swipeMaxDuration;

  void resetStates();
  void appendSequence(uint8_t sensor, unsigned long timestamp);
  uint8_t countBits(uint8_t value) const;
  int8_t firstSetBit(uint8_t value) const;
  bool hasValidSwipeTiming() const;
  bool matchesLeftToRight() const;
  bool matchesRightToLeft() const;
  bool matchesRoundTrip() const;
};

#endif
