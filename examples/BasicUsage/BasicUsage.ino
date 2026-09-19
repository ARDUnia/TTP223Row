/*
  TTP223Row BasicUsage example
  Hamidreza Milaninia [ARDUnia]
*/

#include <TTP223Row.h>

// Four sensors connected from left to right to pins 2, 3, 4, and 5.
TTP223Row touchRow(4, 2, 3, 4, 5);

void setup() {
  Serial.begin(115200);

  if (!touchRow.begin()) {
    Serial.println("Invalid TTP223Row configuration.");
    while (true) {
      // Stop here because the constructor arguments must be corrected.
    }
  }

  // Optional custom timing:
  // touchRow.setTiming(800, 350, 50, 1200);
  Serial.println("TTP223Row is ready.");
}

void loop() {
  String gesture = touchRow.readGesture();

  if (gesture != "") {
    Serial.print("Detected gesture: ");
    Serial.println(gesture);
  }
}
