#include <SPI.h>
#define CS_PIN 5

void setup() {
  Serial.begin(9600);
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  SPI.begin();
}

void loop() {
  //byte dataToSend = Serial.read(); // Letra 'A' ou um inteiro 65
  byte dataToSend = 65;
  Serial.print("Byte: ");
  Serial.println((char)dataToSend);
  SPI.beginTransaction(SPISettings(1500000, MSBFIRST, SPI_MODE0));
  digitalWrite(CS_PIN, LOW); // Coloque CS em baixo, para começar a transição
  byte response = SPI.transfer(dataToSend);
  digitalWrite(CS_PIN, HIGH); // Coloque CS em alto, para terminar a transição
  SPI.endTransaction();
  delay(1000);
}