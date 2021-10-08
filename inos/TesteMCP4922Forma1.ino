#include "AH_MCP4922.h"

//define AnalogOutput (MOSI_pin, SCK_pin, CS_pin, DAC_x, GAIN) 
AH_MCP4922 AnalogOutput1(10,12,9,LOW,HIGH);    //define AnalogOutput1 for MEGA_board, select DAC_A, Gain=1x
AH_MCP4922 AnalogOutput2(10,12,9,HIGH,HIGH);    //define AnalogOutput2 for MEGA_board, select DAC_B, Gain=2x


void setup() { 
  //AnalogOutput1.setValue(4000);
  AnalogOutput2.setValue(0);
  delay(1000);
} 

//*****************************************************************
void loop() {
 
 /* 
  for (int i = 0; i < 4095; i = i + 64) {
    AnalogOutput2.setValue(i);
    delay(10);
  } 
  
  AnalogOutput2.setValue(2048);
  delay(300);
  
  for (int i = 0; i < 10; i++) {
    AnalogOutput2.setValue(4095);
    delay(100);
    AnalogOutput2.setValue(0);
    delay(100);
  } 
  
  AnalogOutput2.setValue(2048);
  delay(300);
    
    
  for (int i = 0; i < 10; i++) {
    AnalogOutput2.setValue(4095);
    delay(100);
    AnalogOutput2.setValue(2048);
    delay(100);
  } 
  
  AnalogOutput2.setValue(2048);
  delay(300);
    
    
  for (int i = 0; i < 10; i++) {
    AnalogOutput2.setValue(2048);
    delay(100);
    AnalogOutput2.setValue(0);
    delay(100);
  } 
  
  AnalogOutput2.setValue(2048);
  delay(300);
  
  */
  
  AnalogOutput2.setValue(4000);
  delay(4000);
  
  AnalogOutput2.setValue(2048);
  delay(2000);
  
  AnalogOutput2.setValue(1100);
  delay(4000);
  
  
  

}

