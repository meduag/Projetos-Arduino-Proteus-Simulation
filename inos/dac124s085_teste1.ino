// inslude the SPI library:
#include <SPI.h>

// set pin 10 as the slave select for the digital pot:
const int NSYNC = 53;

void setup() {
  // set the slaveSelectPin as an output:
  pinMode (NSYNC, OUTPUT);
  // initialize SPI:
  //SPI.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE3));
  SPI.begin();
}

void loop() {

  digitalPotWrite(1, 4095);
  delay(500);
  digitalPotWrite(1, 2048);
  delay(500);
  digitalPotWrite(1, 0);
  delay(500);
}

void digitalPotWrite(int address, int value) {
  // take the SS pin low to select the chip:
  digitalWrite(NSYNC, LOW);
  //  send in the address and value via SPI:
  SPI.transfer(address);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip:
  digitalWrite(NSYNC, HIGH);
}
