#include "AH_MCP4922.h"

//define AnalogOutput (MOSI_pin, SCK_pin, CS_pin, DAC_x, GAIN) 
AH_MCP4922 AnalogOutput1(19,20,21,LOW,HIGH);    //define AnalogOutput1 for MEGA_board, select DAC_A, Gain=1x
AH_MCP4922 AnalogOutput2(19,20,21,HIGH,HIGH);    //define AnalogOutput2 for MEGA_board, select DAC_B, Gain=2x

const int ledPin = 13;

void setup() { 
 pinMode(ledPin, OUTPUT);
} 

//*****************************************************************
void loop() {
  
  AnalogOutput2.setValue(4095);
  digitalWrite(ledPin, HIGH);   // set the LED on
  delay(100);
  AnalogOutput2.setValue(2048);
  delay(100);
  AnalogOutput2.setValue(0);
  digitalWrite(ledPin, LOW);    // set the LED off
  delay(100);

  
}


