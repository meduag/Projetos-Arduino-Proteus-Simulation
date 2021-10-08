#include <SPI.h>

const int cs = 10;///pata teensy 10 chip select

void setup() {
  pinMode(cs, OUTPUT);

  SPI.begin();
  Serial.begin(9600);

  digitalWrite(cs, HIGH);
}

void loop() {

  int mA_Change = 2047;

  enviarvalor(0, 0, mA_Change);
  enviarvalor(1, 0, mA_Change);
  enviarvalor(2, 0, mA_Change);
  enviarvalor(3, 1, mA_Change);

}

void enviarvalor(int DacABCD, int MoP, uint16_t value) {
  byte valuetoWriteH = 0;
  byte valuetoWriteL = 0;

  valuetoWriteH = highByte(value);
  valuetoWriteH = 0b00001111 & valuetoWriteH;
  valuetoWriteH = (DacABCD << 6 ) | (MoP << 4) | valuetoWriteH;
  valuetoWriteL = lowByte(value);

  digitalWrite(cs, LOW);
  SPI.transfer(valuetoWriteH);
  SPI.transfer(valuetoWriteL);
  digitalWrite(cs, HIGH);
}
