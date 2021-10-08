/*
 Example 36.2 - SPI bus device demo using a Microchip MCP4921 DAC [http://bit.ly/j3TSak]
 http://tronixstuff.com/tutorials > chapter 36 | CC by-sa-nc | John Boxall
 */

#include "math.h"
#include "SPI.h" // necessary library
int del=0; // used for various delays
int ss= 53;
int a =0;
word outputValue = 0; // a word is a 16-bit number
byte data = 0; // and a byte is an 8-bit number
float sinVal;
int OsVal;


void setup()
{
  //set pin(s) to input and output
  pinMode(ss, OUTPUT);
  SPI.begin(); // wake up the SPI bus.
  SPI.setBitOrder(MSBFIRST);
}



void loop() 
{

  

 for (int x=30; x<=150; x++) {
    sinVal = (sin(x*(3.1412/180)));
    OsVal = int(sinVal*4095);
   outputValue = OsVal;
    digitalWrite(ss, LOW);
    data = highByte(outputValue);
    data = 0b00001111 & data;
    data = 0b00110000 | data;
    SPI.transfer(data);
    data = lowByte(outputValue);
    SPI.transfer(data);
    digitalWrite(ss, HIGH);
  delay(10);
}

 for (int x=330; x>210; x--) {
    sinVal = (sin(x*(3.1412/180)));
    OsVal = int(sinVal*4095);
   outputValue = OsVal;
    digitalWrite(ss, LOW);
    data = highByte(outputValue);
    data = 0b00001111 & data;
    data = 0b00110000 | data;
    SPI.transfer(data);
    data = lowByte(outputValue);
    SPI.transfer(data);
    digitalWrite(ss, HIGH);
  delay(10);
}


}


