#include <SPI.h>

//const int d2 = 3, d3 = 4, d4 = 5;
//int vd2 = 0, vd3 = 0, vd4 = 0, freq = 30, mA = 0, va = 10, t = 5;
//const int ledPin = 0;

//const int Ct1 = 6, Ct2 = 7, Ct3 = 8, Ct4 = 9;
int mA=0, t=5000;
const int cs = 10;///pata teensy 10 chip select

void setup() {
  // put your setup code here, to run once:
  /*pinMode (d2, INPUT);
  pinMode (d3, INPUT);
  pinMode (d4, INPUT);*/

  //pinMode(ledPin, OUTPUT);

  /*pinMode(Ct1, OUTPUT);
    pinMode(Ct2, OUTPUT);
    pinMode(Ct3, OUTPUT);
    pinMode(Ct4, OUTPUT);*/

  pinMode(cs, OUTPUT);

  SPI.begin();
  //Serial.begin(9600);

  /*digitalWrite(Ct1, LOW);
    digitalWrite(Ct2, LOW);
    digitalWrite(Ct3, LOW);
    digitalWrite(Ct4, LOW);*/

}

void loop() {
  /*  vd2 = digitalRead (d2);
    vd3 = digitalRead (d3);
    vd4 = digitalRead (d4);
    //delay(50);

    if (vd2) {
      // Serial.println("Boton +");
      //digitalWrite(ledPin, HIGH);   // set the LED on
      delay(50);
      if (va == 100) {
        va = 0;
      } else {
        va = va + 1;
      }
    }

    if (vd3) {
      //Serial.println("Boton -");
      //digitalWrite(ledPin, HIGH);   // set the LED on
      //delay(50);
      if (va == 0) {
        va = 1;
      } else {
        va = va - 1;
      }
    }

    if (vd4) {
      //Serial.println("Boton R");
      //digitalWrite(ledPin, HIGH);   // set the LED on
      //delay(50);
      va = 1;
    }

    //digitalWrite(ledPin, LOW);   // set the LED on
    //digitalWrite(Ct2, LOW);

    va = 100;
    mA = map(va, 0, 100, 1, 2047);
  */
  mA = 2048;
  enviarValor(0, 0, 2047 + mA);
  enviarValor(1, 0, 2047 + mA);
  enviarValor(2, 0, 2047 + mA);
  enviarValor(3, 1, 2047 + mA);
  //delayMicroseconds(t);
  delay(t);
  mA = 2047;
  enviarValor(0, 0, 2047 - mA);
  enviarValor(1, 0, 2047 - mA);
  enviarValor(2, 0, 2047 - mA);
  enviarValor(3, 1, 2047 - mA);
  //delayMicroseconds(t);
  delay(t);
  mA = 0;
  enviarValor(0, 0, 2047);
  enviarValor(1, 0, 2047);
  enviarValor(2, 0, 2047);
  enviarValor(3, 1, 2047);
  //delay(freq);
  delay(t);
  //delayMicroseconds(freq);
  //led();
}



void enviarValor(int DacABCD, int MoP, uint16_t value) {
  byte valueToWriteH = 0;
  byte valueToWriteL = 0;

  valueToWriteH = highByte(value);
  valueToWriteH = 0b00001111 & valueToWriteH;
  valueToWriteH = (DacABCD << 6 ) | (MoP << 4) | valueToWriteH;
  valueToWriteL = lowByte(value);

  digitalWrite(cs, LOW);
  SPI.transfer(valueToWriteH);
  SPI.transfer(valueToWriteL);
  digitalWrite(cs, HIGH);
}

//void led(void) {
//  int y = 10;
//  digitalWrite(ledPin, HIGH);   // set the LED on
// delay(y);
//}


