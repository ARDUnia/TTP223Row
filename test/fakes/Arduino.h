#ifndef TEST_FAKE_ARDUINO_H
#define TEST_FAKE_ARDUINO_H

#include <cstdint>
#include <string>
#include <type_traits>

using std::size_t;
using uint8_t = std::uint8_t;
using int8_t = std::int8_t;

#define HIGH 0x1
#define LOW 0x0
#define INPUT 0x0

class String {
 public:
  String() = default;
  String(const char* value) : value_(value ? value : "") {}
  String(const std::string& value) : value_(value) {}

  template <typename T,
            typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
  String(T value) : value_(std::to_string(static_cast<long long>(value))) {}

  String& operator=(const char* value) {
    value_ = value ? value : "";
    return *this;
  }
  String& operator+=(const String& other) {
    value_ += other.value_;
    return *this;
  }
  String& operator+=(const char* other) {
    value_ += other ? other : "";
    return *this;
  }
  bool operator==(const char* other) const {
    return value_ == (other ? other : "");
  }
  bool operator!=(const char* other) const { return !(*this == other); }
  const char* c_str() const { return value_.c_str(); }

 private:
  std::string value_;
};

unsigned long millis();
int digitalRead(uint8_t pin);
void pinMode(uint8_t pin, uint8_t mode);

#endif
