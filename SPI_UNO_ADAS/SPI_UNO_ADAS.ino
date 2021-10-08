#include <SPI.h>
#include "ADAS1000.h"

// set pin 10 as the slave select for the digital pot:
const int CS = 10;

SPISettings conf_tx(2000000, MSBFIRST, SPI_MODE3);

void setup() {
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH); // disable CS

  SPI.begin();
  Serial.begin(115200);
}

void loop() {
  if (Serial.available()) {
    char data = Serial.read();
    Serial.readString(); // Tira o resto do buffer  - hace un flush forzado

    if (data == '1') {
      Serial.println("Modo 1");
      SPI_write(true, 0x81F800BE);
      delayMicroseconds(15);
      Serial.println("Lendo...");
      SPI_read(ADAS1000_ECGCTL);
    }

    if (data == '2') {
      Serial.println("Modo 2 ");
      SPI_write(false, 0x81F800BE);
      delayMicroseconds(15);
      Serial.println("Lendo...");
      SPI_read(ADAS1000_ECGCTL);
    }

  }// Fin serial available
}


/****************************************************************************/
/* Funcion de escritura SPI
*****************************************************************************/
void SPI_write(bool rsy, unsigned long data) {

  unsigned char valueToWrite[4] = {0, 0, 0, 0};

  valueToWrite[0] = (unsigned char)((data & 0xFF000000) >> 24);
  valueToWrite[1] = (unsigned char)((data & 0xFF0000) >> 16);
  valueToWrite[2] = (unsigned char)((data & 0x00FF00) >> 8);
  valueToWrite[3] = (unsigned char)((data & 0x0000FF) >> 0);

  for (int i = 0; i < 4; i++) {
    if (valueToWrite[i] < 16) {
      Serial.print("0");
    }
    Serial.print(valueToWrite[i], HEX);
  }
  Serial.println();

  if (rsy == true) {
    Serial.println("Resync...");
    SPI_resync();
    Serial.println("Lendo Resync...");
    SPI_read(ADAS1000_ECGCTL);
  }


  digitalWrite(CS, LOW); // enable CS

  SPI.beginTransaction(conf_tx);
  for (byte i = 0; i < 4; i++) {
    SPI.transfer(valueToWrite[i]);
  }

  digitalWrite(CS, HIGH); // disable CS
  SPI.endTransaction();

}


/****************************************************************************/
/* Funcion de lectura SPI
*****************************************************************************/
void SPI_read(int adress) {

  unsigned char readCmd[4] = {0, 0, 0, 0};

  SPI_write(false, 0x1000000);

  SPI.beginTransaction(conf_tx);

  digitalWrite(CS, LOW); // enable CS

  for (int i = 0; i < 4; i++) {
    readCmd[i] = SPI.transfer(0);
  }

  digitalWrite(CS, HIGH); // disable CS

  SPI.endTransaction();

  for (int i = 0; i < 4; i++) {
    if (readCmd[i] < 16) {
      Serial.print("0");
    }
    Serial.print(readCmd[i], HEX);
  }

  Serial.println();
}


/****************************************************************************/
/* Funcion de lectura SPI
*****************************************************************************/
void SPI_resync() {

  digitalWrite(CS, HIGH); // disable CS

  // Primeiro só manda os 64 ciclos de clock
  SPI.beginTransaction(conf_tx);

  // Transmite os 64 ciclos de clock
  for (int g = 0; g  < 8; g++) {
    SPI.transfer(0xFF);
  }

  delayMicroseconds(10);

  SPI.endTransaction();
}
