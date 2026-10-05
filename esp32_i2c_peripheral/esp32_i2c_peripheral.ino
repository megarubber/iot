#include <Wire.h>

void setup() {
  Wire.begin(2);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

void loop() {
  delay(1000);
}

void receiveEvent(int numBytes) {
  while (Wire.available()) {
    int x = Wire.read();
    Serial.println(x);
  }
}