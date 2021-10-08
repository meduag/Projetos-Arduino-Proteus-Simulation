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


float base = 2.72;
float x;
double val;
double res;
int y;


 for ( x=0; x<=8.3; x++) {
   
    val = pow(2.72,x) ;
   outputValue = val;
    digitalWrite(ss, LOW);
    data = highByte(outputValue);
    data = 0b00001111 & data;
    data = 0b00110000 | data;
    SPI.transfer(data);
    data = lowByte(outputValue);
    SPI.transfer(data);
    digitalWrite(ss, HIGH);
  delay(1000);
}

}
