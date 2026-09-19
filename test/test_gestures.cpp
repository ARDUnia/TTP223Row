#include <cstdlib>
#include <cstring>
#include <iostream>
#include <initializer_list>

#include "TTP223Row.h"

static unsigned long fakeMillis = 0;
static int pinState[256] = {0};

unsigned long millis() { return fakeMillis; }
int digitalRead(uint8_t pin) { return pinState[pin]; }
void pinMode(uint8_t, uint8_t) {}

static void fail(const char* test, const char* expected, const String& actual) {
  std::cerr << test << ": expected '" << expected << "', got '"
            << actual.c_str() << "'\n";
  std::exit(1);
}

static void expect(const char* test, const char* expected,
                   const String& actual) {
  if (std::strcmp(expected, actual.c_str()) != 0) {
    fail(test, expected, actual);
  }
}

static void clearPins() {
  std::memset(pinState, 0, sizeof(pinState));
}

static void setHigh(std::initializer_list<int> pins) {
  clearPins();
  for (int pin : pins) {
    pinState[pin] = HIGH;
  }
}

static String poll(TTP223Row& row, unsigned long elapsed = 0) {
  fakeMillis += elapsed;
  return row.readGesture();
}

static void testInvalidConfiguration() {
  TTP223Row missingPin(3, 2, 3);
  if (missingPin.isValid() || missingPin.begin()) {
    std::cerr << "invalid configuration was accepted\n";
    std::exit(1);
  }

  TTP223Row duplicatePin(3, 2, 2, 3);
  if (duplicatePin.isValid()) {
    std::cerr << "duplicate pins were accepted\n";
    std::exit(1);
  }
}

static void testMidLengthTouchIsTap() {
  clearPins();
  TTP223Row row(2, 2, 3);
  row.begin();
  setHigh({2});
  poll(row);
  setHigh({});
  poll(row, 700);
  expect("700ms touch", "1TS1", poll(row, 401));
}

static void testSimultaneousLongPress() {
  clearPins();
  TTP223Row row(3, 2, 3, 4);
  row.begin();
  setHigh({2, 3});
  poll(row);
  setHigh({});
  expect("simultaneous long press", "LT2F", poll(row, 1100));
}

static void testStaggeredLongRelease() {
  clearPins();
  TTP223Row row(3, 2, 3, 4);
  row.begin();
  setHigh({2, 3});
  poll(row);
  setHigh({3});
  expect("first staggered release", "", poll(row, 1100));
  setHigh({});
  expect("staggered long press", "LT2F", poll(row, 50));
}

static void testTrueMultiFingerDoubleTap() {
  clearPins();
  TTP223Row row(3, 2, 3, 4);
  row.begin();
  setHigh({2, 3});
  poll(row);
  setHigh({});
  poll(row, 100);
  setHigh({2, 3});
  poll(row, 100);
  setHigh({});
  poll(row, 100);
  expect("two-finger double tap", "2T2F", poll(row, 401));
}

static void testSequentialSensorsAreNotMultiFinger() {
  clearPins();
  TTP223Row row(3, 2, 3, 4);
  row.begin();
  setHigh({2});
  poll(row);
  setHigh({});
  poll(row, 100);
  setHigh({3});
  poll(row, 100);
  setHigh({});
  poll(row, 100);
  expect("sequential sensors", "", poll(row, 401));
}

static void testSwipe() {
  clearPins();
  TTP223Row row(4, 2, 3, 4, 5);
  row.begin();
  setHigh({2});
  poll(row);
  setHigh({3});
  poll(row, 100);
  setHigh({4});
  poll(row, 100);
  setHigh({5});
  poll(row, 100);
  setHigh({});
  poll(row, 100);
  expect("left-to-right swipe", "L2R", poll(row, 401));
}

static void testSimultaneousTouchIsNotSwipe() {
  clearPins();
  TTP223Row row(4, 2, 3, 4, 5);
  row.begin();
  setHigh({2, 3, 4, 5});
  poll(row);
  setHigh({});
  poll(row, 200);
  expect("simultaneous four-finger touch", "1T4F", poll(row, 401));
}

static void testSlowSequenceIsNotSwipe() {
  clearPins();
  TTP223Row row(4, 2, 3, 4, 5);
  row.begin();
  setHigh({2});
  poll(row);
  setHigh({3});
  poll(row, 600);
  setHigh({4});
  poll(row, 600);
  setHigh({5});
  poll(row, 600);
  setHigh({});
  poll(row, 100);
  expect("slow sequence", "", poll(row, 401));
}

static void testCustomTiming() {
  clearPins();
  TTP223Row row(2, 2, 3);
  if (!row.setTiming(600, 300, 50, 1000) ||
      row.setTiming(0, 300, 50, 1000)) {
    std::cerr << "timing validation failed\n";
    std::exit(1);
  }
  row.begin();
  setHigh({2});
  poll(row);
  setHigh({});
  expect("custom long-press timing", "LT1F_1", poll(row, 700));
}

int main() {
  testInvalidConfiguration();
  testMidLengthTouchIsTap();
  testSimultaneousLongPress();
  testStaggeredLongRelease();
  testTrueMultiFingerDoubleTap();
  testSequentialSensorsAreNotMultiFinger();
  testSwipe();
  testSimultaneousTouchIsNotSwipe();
  testSlowSequenceIsNotSwipe();
  testCustomTiming();
  std::cout << "All TTP223Row host tests passed.\n";
  return 0;
}
