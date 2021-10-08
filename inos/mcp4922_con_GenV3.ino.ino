#include "AH_MCP4922.h"

AH_MCP4922 AnalogOutput1(21, 22, 23, HIGH, HIGH); //define AnalogOutput1 for MEGA_board, select DAC_A, Gain=1x High
AH_MCP4922 AnalogOutput2(21, 22, 23, LOW, HIGH); //define AnalogOutput2 for MEGA_board, select DAC_B, Gain=2x Low

const int ledPin = 13;
int red = 1;
//int ccor = -400;

void setup() {
  pinMode(ledPin, OUTPUT);
}

//*****************************************************************
void loop() {

      AnalogOutput1.setValue(4095);
      AnalogOutput2.setValue(4095);
      //digitalWrite(ledPin, HIGH);   // set the LED on
      delay(red);
      AnalogOutput1.setValue(2048);
      AnalogOutput2.setValue(2048);
      delay(red);
      //digitalWrite(ledPin, LOW);   // set the LED on
      AnalogOutput1.setValue(0);
      AnalogOutput2.setValue(0);
      //digitalWrite(ledPin, HIGH);   // set the LED on
      delay(red);
      AnalogOutput1.setValue(2048);
      AnalogOutput2.setValue(2048);
      //digitalWrite(ledPin, LOW);   // set the LED on
      delay(red);

}


