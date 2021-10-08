#include <SPI.h>

//int ss = 53; //para arduino mega
//pata teensy el chip select es el 10
int ss = 10;

//endereço do dac
int dac = 0;
//Modo de operacion
int modop  = 1;

//Frequencia de operacion
int freq = 50;

//Valor en miliamperios
int va = 0;
int mA = 0;

//Tiempo on
int t = 10;

void setup() {
  //Configuracion del pin
  pinMode(ss, OUTPUT);
  SPI.begin();
  //Serial.begin(9600);
}

void loop() {

  //Calculo de frecuencia
  //float fq = 1 / freq;
  //int fq2 = fq * 1000000;

  //valor real en miliamperios
  va = 50;
  mA = map(va, 0, 100, 1, 2047);

  //Calculo de frecuencias
  //freq = fq2 - (t * 2);
  freq = t * 2;
  //dac = ;
  int mA2 = mA * 0.02;
  //int mA2 = 0;

  //enviarValor(dac, 1, 2047 + mA);
  enviarValor(0, 1, 2047 + mA);
  enviarValor(1, 1, 2047 + mA);
  enviarValor(2, 1, 2047 + mA);
  enviarValor(3, 1, 2047 + mA);
  delay(t);
  //delayMicroseconds(t);
  enviarValor(0, 1, 2047 - (mA - mA2));
  enviarValor(1, 1, 2047 - (mA - mA2));
  enviarValor(2, 1, 2047 - (mA - mA2));
  enviarValor(3, 1, 2047 - (mA - mA2));
  delay(t);
  //delayMicroseconds(t);
  enviarValor(0, 1, 2047);
  enviarValor(1, 1, 2047);
  enviarValor(2, 1, 2047);
  enviarValor(3, 1, 2047);
  delay(freq);

  /*enviarValor(0, 1, mA2);
  enviarValor(1, 1, mA2);
  enviarValor(2, 1, mA2);
  enviarValor(3, 1, mA2);*/
  //delayMicroseconds(freq);

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
