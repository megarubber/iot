#include <ESP32Servo.h>
#include <Adafruit_ADS1X15.h>
#include <Wire.h>

Adafruit_ADS1115 ads;

Servo servo;

#define MOTOR_PIN 18

void setup() {
  Serial.begin(9600);
  servo.attach(MOTOR_PIN);
  Wire.begin();
  while(!ads.begin(0x48)) Serial.println("Failed to initialize.");
  Serial.println("Start ADS1115.");
}

void loop() { 
  int16_t adc0;
  adc0 = ads.readADC_SingleEnded(0);
  Serial.print("ADC0 Raw: "); 
  Serial.println(adc0);
  
  int angle = map(adc0, 13000, 17300, 0, 180);
  //angle = constrain(angle, 0, 180);
  
  servo.write(angle);
  
  Serial.print("Servo Angle: "); 
  Serial.println(angle);

  delay(1000);
}
