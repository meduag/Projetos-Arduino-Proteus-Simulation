#include <Arduino.h>
#include "SPI.h"

const byte chipselect = 10;
const byte num_bytes = 2;

unsigned long agora = 0;
int delayt = 5000;

void setup() {
  pinMode(chipselect, OUTPUT);
  digitalWrite(chipselect, HIGH);

  //spi4teensy3::init(1, 0, 0 );
  SPI.begin();
  Serial.begin(115200);
}

void loop() {
  SPI_write(0, 1, 4095);
  /*agora = millis();
  while (millis() < agora + delayt) {}
  SPI_write(0, 1, 0);
  agora = millis();
  while (millis() < agora + delayt) {}*/
}

void SPI_write(int address, int operation_mode, uint16_t value) {
  unsigned char writeCmd[num_bytes] = {0, 0};
  unsigned char readCmd[num_bytes] = {0, 0};

  writeCmd[0] = highByte(value);
  writeCmd[0] = 0b00001111 & writeCmd[0];
  writeCmd[0] = (address << 6 ) | (operation_mode << 4) | writeCmd[0];
  writeCmd[1] = lowByte(value);

  Serial.println("Write SPI");
  byte i;
  for (i = 0; i < num_bytes; i++) {
    Serial.print("writeCmd[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(writeCmd[i], BIN);
  }

  digitalWrite(chipselect, LOW); // enable shifting
  //spi4teensy3::send(writeCmd, num_bytes);
  for (i = 0; i < num_bytes; i++) {
    readCmd[i] = SPI.transfer(writeCmd);
  }
  digitalWrite(chipselect, HIGH); // disable clock again*/
}
