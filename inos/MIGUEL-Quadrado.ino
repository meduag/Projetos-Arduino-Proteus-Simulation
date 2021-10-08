/*
 Example 36.2 - SPI bus device demo using a Microchip MCP4921 DAC [http://bit.ly/j3TSak]
 http://tronixstuff.com/tutorials > chapter 36 | CC by-sa-nc | John Boxall
 */

#include "math.h"
#include "SPI.h" // necessary library
int del=0; // used for various delays
int ss= 53;
int a =3098;
word outputValue = 0; // a word is a 16-bit number
byte data = 0; // and a byte is an 8-bit number
void setup()
{
  //set pin(s) to input and output
  pinMode(ss, OUTPUT);
  SPI.begin(); // wake up the SPI bus.
  SPI.setBitOrder(MSBFIRST);
}

void delay250ns()
{
  volatile int x=0;
  return;
}

void Onda_Quadrada(float v){
  
   outputValue = 4095;
    digitalWrite(ss, LOW);
    data = highByte(outputValue);
    data = 0b00001111 & data;
    data = 0b00110000 | data;
    SPI.transfer(data);
    data = lowByte(outputValue);
    SPI.transfer(data);
    digitalWrite(ss, HIGH);
    delay(1000);
    //delayMicroseconds(1);
    //delay250ns();
  //}
  //delay(del+25);
  
    outputValue = 0;
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
  

void loop ()
{
  
    outputValue = 4095;
    digitalWrite(ss, LOW);
    data = highByte(outputValue);
    data = 0b00001111 & data;
    data = 0b00110000 | data;
    SPI.transfer(data);
    data = lowByte(outputValue);
    SPI.transfer(data);
    digitalWrite(ss, HIGH);
    delay(1000);
    //delayMicroseconds(1);
    //delay250ns();
  //}
  //delay(del+25);
  
    outputValue = 0;
    digitalWrite(ss, LOW);
    data = highByte(outputValue);
    data = 0b00001111 & data;
    data = 0b00110000 | data;
    SPI.transfer(data);
    data = lowByte(outputValue);
    SPI.transfer(data);
    digitalWrite(ss, HIGH);
    delay(1000);
   
    
  
 /* for ( a=4095; a>=0; --a)
  {
    outputValue = a;
    digitalWrite(ss, LOW);
    data = highByte(outputValue);
    data = 0b00001111 & data;
    data = 0b00110000 | data;
    SPI.transfer(data);
    data = lowByte(outputValue);
    SPI.transfer(data);
    digitalWrite(ss, HIGH);
    delayMicroseconds(1);
    //delay250ns();
  } */
  //delay(del+25);
}
