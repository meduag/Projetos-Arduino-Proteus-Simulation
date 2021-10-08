#include "SPI.h" // necessary library
int del=50; // used for various delays
word outputValue = 0; // a word is a 16-bit number
byte data = 0; // and a byte is an 8-bit number
void setup()
{
  //set pin(s) to input and output
  pinMode(53, OUTPUT);
  SPI.begin(); // wake up the SPI bus.
  SPI.setBitOrder(MSBFIRST);
}
 
void loop()
{
  for (int a=0; a<=4095; a++)
  {
    outputValue = a;
    digitalWrite(53, LOW);
    data = highByte(outputValue);
    data = 0b00001111 & data;
    data = 0b00110000 | data;
    SPI.transfer(data);
    data = lowByte(outputValue);
    SPI.transfer(data);
    digitalWrite(53, HIGH);
    delayMicroseconds(del);
  }
  delayMicroseconds(100);
  
  for (int a=4095; a>=0; --a)
  {
    outputValue = a;
    digitalWrite(53, LOW);
    data = highByte(outputValue);
    data = 0b00001111 & data;
    data = 0b00110000 | data;
    SPI.transfer(data);
    data = lowByte(outputValue);
    SPI.transfer(data);
    digitalWrite(53, HIGH);
    delayMicroseconds(del);
  }
  delayMicroseconds(100);
}
