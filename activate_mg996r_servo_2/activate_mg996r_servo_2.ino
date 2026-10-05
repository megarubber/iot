#include <ESP32Servo.h>
#include <Adafruit_ADS1X15.h>
#include <Wire.h>

Adafruit_ADS1115 ads;

Servo servo;

#define LDR_PIN 34
#define MOTOR_PIN 9

void setup() {
  servo.attach(MOTOR_PIN);
  Serial.begin(9600);
  pinMode(LDR_PIN, INPUT);
  Wire.begin();
}

void loop() {
  int r = analogRead(LDR_PIN);
  int angle = map(r, 0, 4065, 0, 180);

  servo.write(angle);
  Serial.println(r);

  delay(1000);
}
