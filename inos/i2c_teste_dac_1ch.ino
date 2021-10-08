#include <Wire.h>

byte valueToWriteH = 0;
byte valueToWriteL = 0;


void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {

  enviar(0);
  delay(2000);
  enviar(4095);
  delay(2000);

}

void enviar(uint16_t value) {
  valueToWriteH = 0;
  valueToWriteL = 0;

  valueToWriteH = highByte(value);
  valueToWriteH = 0b00001111 & valueToWriteH;
  //valueToWriteH = (address << 6) | (operation_mode << 4) | valueToWriteH;
  valueToWriteL = lowByte(value);

  Wire.beginTransmission(0b00001001);
  //Wire.send(valueToWriteH);  // address low byte
  //Wire.send(valueToWriteL);  // address high byte
  Wire.write(valueToWriteH);  // address low byte
  Wire.write(valueToWriteL);  // address high byte
  Wire.endTransmission();
}

