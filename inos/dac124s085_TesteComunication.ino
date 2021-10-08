#include <SPI.h>


word outputValue = 0; // 16-bit
byte data = 0; // 8-bit
//int ss = 53; //para arduino mega
int ss = 10;///pata teensy 10
int dac = 0;
int modop  = 1;
int val = 500;

int Ton = 500;
int freq = 50;


void setup() {
  pinMode(ss, OUTPUT);
  //SPI.setMOSI(11);//estava en 11
  //SPI.setSCK(13);//estava en 13
  SPI.begin();
  Serial.begin(9600);
}

void loop() {
   int va = 17;
  int mA = map(va, 0, 100, 1, 2047);

  int t = 10;

  dac = 0;

  enviarValor(dac, 1, 2048 + mA);
  delay(t);
  //delayMicroseconds(t);
  enviarValor(dac, 1, 2047 - mA);
  delay(t);
  //delayMicroseconds(t);
  enviarValor(dac, 1, 2048);
  delay(t * 2);




}

void miliamp(int mA) {
  enviarValor(dac, modop, 2048 + mA);
  delayMicroseconds(Ton);
  enviarValor(dac, modop, 2048 - mA);
  delayMicroseconds(Ton);
  enviarValor(dac, modop, 2048);
  delayMicroseconds(freq);
}


void enviarValor(int DacABCD, int MoP, uint16_t value) {
  //delay(300);
  byte valueToWriteH = 0;
  byte valueToWriteL = 0;

  /*Serial.print("changed ");
  Serial.print(DacABCD); Serial.print(" ");
  Serial.print(MoP); Serial.print(" ");
  Serial.print(value); Serial.print(" ");
  Serial.print(" = ");*/

  valueToWriteH = highByte(value);
  /*Serial.print("\n Valor alto ");
  Serial.print(valueToWriteH, BIN);*/
  valueToWriteH = 0b00001111 & valueToWriteH;
  /*Serial.print("\n Valor AND ");
  Serial.print(valueToWriteH, BIN);*/

  valueToWriteH = (DacABCD << 6 ) | (MoP << 4) | valueToWriteH;
  /*Serial.print("\n Valor alto ");
  Serial.print(valueToWriteH, BIN);*/



  valueToWriteL = lowByte(value);
  //  Serial.print("\n Valor bajo ");
  //  Serial.print(valueToWriteL, BIN);
  //  Serial.print("\n Fin \n\n\n");

  // take the SS pin low to select the chip:
  digitalWrite(ss, LOW);
  //  send in the address and value via SPI:
  SPI.transfer(valueToWriteH);
  SPI.transfer(valueToWriteL);
  // take the SS pin high to de-select the chip:
  digitalWrite(ss, HIGH);
}
