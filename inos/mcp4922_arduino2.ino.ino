#include "AH_MCP4922.h"

//define AnalogOutput (MOSI_pin, SCK_pin, CS_pin, DAC_x, GAIN) 
AH_MCP4922 AnalogOutput1(21,22,23,LOW,HIGH);    //define AnalogOutput1 for MEGA_board, select DAC_A, Gain=1x
AH_MCP4922 AnalogOutput2(21,22,23,HIGH,HIGH);    //define AnalogOutput2 for MEGA_board, select DAC_B, Gain=2x

const int ledPin = 13;
int red = 5;
//String datosIn = "";

void setup() { 
 /* Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(13,LOW);
  delay(1000);
  digitalWrite(13,HIGH);
  delay(2000);
  digitalWrite(13,LOW);*/
} 

//*****************************************************************
void loop() {
 /* if(Serial.available()>0){
    datosIn = Serial.read();
    
      digitalWrite(13,LOW);
      delay(1000);
      digitalWrite(13,HIGH);
      delay(2000);
      digitalWrite(13,LOW);
    
    
      if (datosIn == '13'){
            digitalWrite(13,HIGH);
      }
  }*/
  
  AnalogOutput2.setValue(4095);
  digitalWrite(ledPin, HIGH);   // set the LED on
  delay(red);
  //AnalogOutput2.setValue(2048);
  //delay(red);
  AnalogOutput2.setValue(0);
  digitalWrite(ledPin, LOW);    // set the LED off
  delay(red);
  AnalogOutput2.setValue(2048);
  delay(red);
}


