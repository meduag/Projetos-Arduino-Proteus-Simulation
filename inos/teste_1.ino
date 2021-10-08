#include <SPI.h>

const int cs = 10;///pata teensy 10 chip select

void setup() {
  // put your setup code here, to run once:
  pinMode(cs, OUTPUT);
  SPI.begin();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

}
