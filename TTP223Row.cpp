/*
  TTP223Row.cpp
  Copyright (c) 2026 Hamidreza Milaninia [ARDUnia]
  Version 1.1.0
*/

#include "TTP223Row.h"

TTP223Row::TTP223Row(uint8_t num, int p1, int p2, int p3, int p4, int p5)
    : _numSensors(0),
      _valid(false),
      _begun(false),
      _longPressMin(1000),
      _multiTapGap(400),
      _swipeMinDuration(60),
      _swipeMaxDuration(1500) {
  const int requestedPins[MAX_SENSORS] = {p1, p2, p3, p4, p5};

  for (uint8_t i = 0; i < MAX_SENSORS; ++i) {
    _pins[i] = 0;
    _touchStartTime[i] = 0;
    _isTouched[i] = false;
    _wasTouched[i] = false;
  }

  if (num < 2 || num > MAX_SENSORS) {
    resetStates();
    return;
  }

  for (uint8_t i = 0; i < num; ++i) {
    if (requestedPins[i] < 0 || requestedPins[i] > 255) {
      resetStates();
      return;
    }
    for (uint8_t j = 0; j < i; ++j) {
      if (requestedPins[i] == requestedPins[j]) {
        resetStates();
        return;
      }
    }
  }

  _numSensors = num;
  for (uint8_t i = 0; i < _numSensors; ++i) {
    _pins[i] = static_cast<uint8_t>(requestedPins[i]);
  }
  _valid = true;
  resetStates();
}

bool TTP223Row::begin() {
  if (!_valid) {
    return false;
  }

  for (uint8_t i = 0; i < _numSensors; ++i) {
    pinMode(_pins[i], INPUT);
  }
  resetStates();
  _begun = true;
  return true;
}

bool TTP223Row::isValid() const { return _valid; }

bool TTP223Row::setTiming(unsigned long longPressMin,
                          unsigned long multiTapGap,
                          unsigned long swipeMinDuration,
                          unsigned long swipeMaxDuration) {
  if (longPressMin == 0 || multiTapGap == 0 || swipeMinDuration == 0 ||
      swipeMinDuration >= swipeMaxDuration) {
    return false;
  }

  _longPressMin = longPressMin;
  _multiTapGap = multiTapGap;
  _swipeMinDuration = swipeMinDuration;
  _swipeMaxDuration = swipeMaxDuration;
  resetStates();
  return true;
}

void TTP223Row::resetStates() {
  for (uint8_t i = 0; i < MAX_SENSORS; ++i) {
    _touchStartTime[i] = 0;
    _isTouched[i] = false;
    _wasTouched[i] = false;
  }

  _sequenceLength = 0;
  _sequenceOverflow = false;
  _previousActiveTouches = 0;
  _currentGroupMask = 0;
  _currentGroupMaxActive = 0;
  _tapPatternMask = 0;
  _tapGroupCount = 0;
  _tapPatternConsistent = true;
  _longPressMask = 0;
  _globalGestureTimer = 0;
}

void TTP223Row::appendSequence(uint8_t sensor, unsigned long timestamp) {
  if (_sequenceLength > 0 && _sequence[_sequenceLength - 1] == sensor) {
    return;
  }
  if (_sequenceLength >= MAX_SEQUENCE) {
    _sequenceOverflow = true;
    return;
  }
  _sequence[_sequenceLength] = sensor;
  _sequenceTime[_sequenceLength] = timestamp;
  ++_sequenceLength;
}

uint8_t TTP223Row::countBits(uint8_t value) const {
  uint8_t count = 0;
  while (value != 0) {
    count += value & 1U;
    value >>= 1U;
  }
  return count;
}

int8_t TTP223Row::firstSetBit(uint8_t value) const {
  for (uint8_t i = 0; i < _numSensors; ++i) {
    if (value & (1U << i)) {
      return static_cast<int8_t>(i);
    }
  }
  return -1;
}

bool TTP223Row::hasValidSwipeTiming() const {
  if (_sequenceOverflow || _sequenceLength < 2) {
    return false;
  }
  const unsigned long duration =
      _sequenceTime[_sequenceLength - 1] - _sequenceTime[0];
  return duration >= _swipeMinDuration && duration <= _swipeMaxDuration;
}

bool TTP223Row::matchesLeftToRight() const {
  if (_sequenceLength != _numSensors) {
    return false;
  }
  for (uint8_t i = 0; i < _numSensors; ++i) {
    if (_sequence[i] != i) {
      return false;
    }
  }
  return true;
}

bool TTP223Row::matchesRightToLeft() const {
  if (_sequenceLength != _numSensors) {
    return false;
  }
  for (uint8_t i = 0; i < _numSensors; ++i) {
    if (_sequence[i] != (_numSensors - 1U - i)) {
      return false;
    }
  }
  return true;
}

bool TTP223Row::matchesRoundTrip() const {
  const uint8_t expectedLength = 2U * _numSensors - 1U;
  if (_sequenceLength != expectedLength) {
    return false;
  }

  for (uint8_t i = 0; i < _numSensors; ++i) {
    if (_sequence[i] != i) {
      return false;
    }
  }
  for (uint8_t i = _numSensors; i < expectedLength; ++i) {
    const uint8_t expected = 2U * _numSensors - 2U - i;
    if (_sequence[i] != expected) {
      return false;
    }
  }
  return true;
}

String TTP223Row::readGesture() {
  if (!_valid || !_begun) {
    return String();
  }

  const unsigned long currentMillis = millis();
  uint8_t activeTouches = 0;
  uint8_t currentMask = 0;

  for (uint8_t i = 0; i < _numSensors; ++i) {
    _isTouched[i] = digitalRead(_pins[i]) == HIGH;
    if (_isTouched[i]) {
      ++activeTouches;
      currentMask |= (1U << i);
    }
  }

  if (_previousActiveTouches == 0 && activeTouches > 0) {
    _currentGroupMask = currentMask;
    _currentGroupMaxActive = activeTouches;
  } else if (activeTouches > 0) {
    _currentGroupMask |= currentMask;
    if (activeTouches > _currentGroupMaxActive) {
      _currentGroupMaxActive = activeTouches;
    }
  }

  for (uint8_t i = 0; i < _numSensors; ++i) {
    if (_isTouched[i] && !_wasTouched[i]) {
      _touchStartTime[i] = currentMillis;
      _globalGestureTimer = currentMillis;
      appendSequence(i, currentMillis);
    }

    if (!_isTouched[i] && _wasTouched[i]) {
      const unsigned long touchDuration =
          currentMillis - _touchStartTime[i];
      if (touchDuration >= _longPressMin) {
        _longPressMask |= (1U << i);
      }
      _globalGestureTimer = currentMillis;
    }
    _wasTouched[i] = _isTouched[i];
  }

  if (_previousActiveTouches > 0 && activeTouches == 0) {
    if (_longPressMask != 0) {
      const uint8_t longFingerCount = countBits(_longPressMask);
      String result = "LT";
      result += String(longFingerCount);
      result += "F";
      if (longFingerCount == 1) {
        result += "_";
        result += String(firstSetBit(_longPressMask) + 1);
      }
      resetStates();
      return result;
    }

    const bool groupIsUnambiguous =
        countBits(_currentGroupMask) == _currentGroupMaxActive;
    if (_tapGroupCount == 0) {
      _tapPatternMask = _currentGroupMask;
    } else if (_tapPatternMask != _currentGroupMask) {
      _tapPatternConsistent = false;
    }
    if (!groupIsUnambiguous) {
      _tapPatternConsistent = false;
    }
    ++_tapGroupCount;
    _globalGestureTimer = currentMillis;
  }

  _previousActiveTouches = activeTouches;

  if (activeTouches == 0 && _sequenceLength > 0 &&
      currentMillis - _globalGestureTimer > _multiTapGap) {
    String result;

    // A swipe must be one continuous touch group with an exact sequence.
    if (_tapGroupCount == 1 && hasValidSwipeTiming()) {
      if (matchesRoundTrip()) {
        result = "RLLR";
      } else if (matchesLeftToRight()) {
        result = "L2R";
      } else if (matchesRightToLeft()) {
        result = "R2L";
      }
    }

    if (result == "" && _tapGroupCount > 0 && _tapPatternConsistent) {
      const uint8_t fingerCount = countBits(_tapPatternMask);
      if (fingerCount == 1) {
        result = String(_tapGroupCount);
        result += "TS";
        result += String(firstSetBit(_tapPatternMask) + 1);
      } else if (fingerCount > 1) {
        result = String(_tapGroupCount);
        result += "T";
        result += String(fingerCount);
        result += "F";
      }
    }

    resetStates();
    return result;
  }

  return String();
}

String TTP223Row::HowTouch() { return readGesture(); }
