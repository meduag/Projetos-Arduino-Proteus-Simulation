#include <SPI.h>

#define cs 10
byte e = 0b10000001;

void setup() {
  SPI.begin();
  pinMode(cs, OUTPUT);
  digitalWrite(cs, 1);
}

void loop() {
  digitalWrite(cs, 0);
  SPI.transfer(e);
  digitalWrite(cs, 1);
}
