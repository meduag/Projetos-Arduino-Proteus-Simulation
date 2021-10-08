//#include "SPI.h"

int ss = 53;
int T1 = 500;
int T2 = 2000;
int mA = 10;

int dac = 0;
int modop = 1;

int pinc = 22;

byte valueToWriteH = 0;//Variable Byte alto
byte valueToWriteL = 0;//Variable Byte bajo

void setup() {

  DDRA = B11111111;
  //PORTA = B10101000;

 /* pinMode(ss, OUTPUT);
  pinMode(pinc, OUTPUT);
  SPI.begin();

  digitalWrite(ss, HIGH);
  digitalWrite(pinc, LOW);

  enviarValor(dac, modop, 2048);
  delay(1000);

  mA = map(mA, 0, 100, 0 , 2047);
  //mA = 1000;
*/
}

void loop() {
  /*
    digitalWrite(pinc, HIGH);
    enviarValor(dac, modop, 2047 + mA);
    delayMicroseconds(T1);
    enviarValor(dac, modop, 2047 - mA);
    delayMicroseconds(T1);
    digitalWrite(pinc, LOW);
    enviarValor(dac, modop, 2047);
    delayMicroseconds(T2);
  */

  PORTA = B00000000;
  delay(200);
  PORTA = B11111111;
  delay(200);



}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////****  Funcion para enviar los datos al DAC por SPI  ****////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*void enviarValor(int DacABCD, int MoP, uint16_t value) {

  valueToWriteH = highByte(value);//Separo el byte alto
  valueToWriteH = 0b00001111 & valueToWriteH;
  valueToWriteH = (DacABCD << 6 ) | (MoP << 4) | valueToWriteH;

  valueToWriteL = lowByte(value);//Separo el byte bajo

  digitalWrite(ss, LOW);//Activo el SPI device para recibor los dados
  SPI.transfer(valueToWriteH);//Envio byte alto
  SPI.transfer(valueToWriteL);//Envio byte bajo
  digitalWrite(ss, HIGH);//Desactivo el SPI device
}*/
