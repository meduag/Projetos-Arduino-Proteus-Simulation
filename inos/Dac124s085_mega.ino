#include <SPI.h>

int ss = 53; //para arduino mega
int dac = 0;

void setup() {
  pinMode(ss, OUTPUT);
  SPI.begin();
  //Serial.begin(9600);
}

void loop() {

  int va = 100;
  int mA = map(va, 0, 100, 1, 2047);
  int t = 50;
  dac = 0;

  enviarValor(dac, 1, 2048 + mA);
  delay(t);
  //delayMicroseconds(t);
  enviarValor(dac, 1, 2048 - mA);
  delay(t);
  //delayMicroseconds(t);
  enviarValor(dac, 1, 2048);
  delay(t*2);
  //delayMicroseconds(t);

  /*for (int i = 50; i <= 100; i ++) {

    mA = map(i, 0, 100, 1, 2047);

    for (int j = 1; j <= 10; j ++) {
    enviarValor(0, 1, 2048 + mA);
    delay(t);
    enviarValor(0, 1, 2048 - mA);
    delay(t);
    enviarValor(0, 1, 2048);
    delay(t);
    }

  }*/
}

/*void miliamp(int mA) {
  enviarValor(dac, modop, 2048 + mA);
  delayMicroseconds(Ton);
  enviarValor(dac, modop, 2048 - mA);
  delayMicroseconds(Ton);
  enviarValor(dac, modop, 2048);
  delayMicroseconds(freq);
}*/


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
