#include <SPI.h>

void setup()
{
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  SPI.begin();

  sendStimValue(0, 1, 4095);
}

void loop()
{
}

void sendStimValue(int address, int operation_mode, uint16_t value)
{
  byte valueToWriteH = 0;
  byte valueToWriteL = 0;
  
  valueToWriteH = highByte(value);
  valueToWriteH = 0b00001111 & valueToWriteH;
  valueToWriteH = (address << 6 ) | (operation_mode << 4) | valueToWriteH;
  valueToWriteL = lowByte(value);
  
  // take the SS pin low to select the chip:
  digitalWrite(10, LOW);
  
  //  send in the address and value via SPI:
  SPI.transfer(valueToWriteH);
  SPI.transfer(valueToWriteL);
  
  // take the SS pin high to de-select the chip:
  digitalWrite(10, HIGH);
}
