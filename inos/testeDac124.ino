#include <SPI.h>

int ss = 10;
int t = 1000;

//int ss1 = 8;

void setup() {
  pinMode(ss, OUTPUT);
  //pinMode(ss1, OUTPUT);
  digitalWrite(ss, HIGH);
  SPI.begin();
}

void loop() {
  //digitalWrite(ss1, LOW);
  enviarValor(0, 1, 4095);
  delay(t);
  //digitalWrite(ss1, HIGH);
  enviarValor(0, 1, 2047);
  delay(t);
  //digitalWrite(ss1, LOW);
  enviarValor(0, 1, 0);
  delay(t);
  //digitalWrite(ss1, HIGH);
}


void enviarValor(int DacABCD, int MoP, uint16_t value) {
  byte valueToWriteH = 0;
  byte valueToWriteL = 0;

  valueToWriteH = highByte(value);
  valueToWriteH = 0b00001111 & valueToWriteH;
  valueToWriteH = (DacABCD << 6 ) | (MoP << 4) | valueToWriteH;

  valueToWriteL = lowByte(value);

  // take the SS pin low to select the chip:
  digitalWrite(ss, LOW);
  //  send in the address and value via SPI:
  SPI.transfer(valueToWriteH);
  SPI.transfer(valueToWriteL);
  // take the SS pin high to de-select the chip:
  digitalWrite(ss, HIGH);
}
