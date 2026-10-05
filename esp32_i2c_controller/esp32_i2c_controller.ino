#include <Wire.h>

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

byte x = 5;
int peripheal = 2;

void loop() {
  Wire.beginTransmission(peripheal);
  Wire.write(x);
  Serial.print("Write ");
  Serial.print((int)x);
  Serial.print(" for ");
  Serial.println(peripheal);
  Wire.endTransmission();
  delay(1000);
}