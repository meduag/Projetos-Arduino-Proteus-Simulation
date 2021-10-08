#include <SPI.h>

word outputValue = 0; // 16-bit
byte data = 0; // 8-bit
int ss = 53;
int dac = 0;
int modop  = 0;


void setup() {
  pinMode(ss, OUTPUT);
  //SPI.setMOSI(20);//estava en 11
  //SPI.setSCK(21);//estava en 13
  SPI.begin();
  //Serial.begin(9600);
}

void loop() {
  dac = 0;
  modop = 0;
  // put your main code here, to run repeatedly:
  enviarValor(dac, modop, 4095);
  delay(400);
  enviarValor(dac, modop, 2048);
  delay(200);
  enviarValor(dac, modop, 0);
  delay(200);
  enviarValor(dac, modop, 2048);
  delay(200);
  enviarValor(dac, modop, 0);
}

void enviarValor(int DacABCD, int MoP, uint16_t value) {
  byte valueToWriteH = 0;
  byte valueToWriteL = 0;

  /*Serial.print("changed ");
  Serial.print(DacABCD); Serial.print(" ");
  Serial.print(MoP); Serial.print(" ");
  Serial.print(value); Serial.print(" ");
  Serial.print(" = ");*/

  valueToWriteH = highByte(value);
  //Serial.print("\n Valor alto ");
  //Serial.print(valueToWriteH, BIN);
  valueToWriteH = 0b00001111 & valueToWriteH;
  //Serial.print("\n Valor AND ");
  //Serial.print(valueToWriteH, BIN);

  valueToWriteH = (DacABCD << 6 ) | (MoP << 4) | valueToWriteH;
 // Serial.print("\n Valor alto ");
  //Serial.print(valueToWriteH, BIN);



  valueToWriteL = lowByte(value);
 /* Serial.print("\n Valor bajo ");
  Serial.print(valueToWriteL, BIN);
  Serial.print("\n Fin \n\n\n");*/

  // take the SS pin low to select the chip:
  digitalWrite(ss, LOW);
  //  send in the address and value via SPI:
  SPI.transfer(valueToWriteH);
  SPI.transfer(valueToWriteL);
  // take the SS pin high to de-select the chip:
  digitalWrite(ss, HIGH);
  
}
