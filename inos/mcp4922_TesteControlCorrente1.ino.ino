#include "AH_MCP4922.h"

//define AnalogOutput (MOSI_pin, SCK_pin, CS_pin, DAC_x, GAIN)
AH_MCP4922 AnalogOutput1(15, 16, 17, LOW, HIGH); //define AnalogOutput1 for MEGA_board, select DAC_A, Gain=1x
AH_MCP4922 AnalogOutput2(15, 16, 17, HIGH, HIGH); //define AnalogOutput2 for MEGA_board, select DAC_B, Gain=2x

const int ledPin = 13;
int red = 200;
int ccor = 50;

void setup() {
  pinMode(ledPin, OUTPUT);
}

//*****************************************************************
void loop() {

 // for (int j = 0; j <= 7; j++) {

/*    for (int i = 0; i <= 6; i++) {
      AnalogOutput2.setValue(2048 + ccor);
      digitalWrite(ledPin, HIGH);   // set the LED on
      delay(red);
      AnalogOutput2.setValue(2048);
      delay(red);
      AnalogOutput2.setValue(2048 - ccor);
      digitalWrite(ledPin, LOW);    // set the LED off
      delay(red);
      AnalogOutput2.setValue(2048);
      delay(red);

      ccor = ccor +60;

      if (ccor > 350) {
        ccor = 0;
      }*/

   // } //fin muda a corrente

    //red = red - 90;

   /* if (red < 50) {
      red = 500;
    }*/
    //ccor = 0;


      AnalogOutput2.setValue(4095);
      digitalWrite(ledPin, HIGH);   // set the LED on
      delay(red);
      AnalogOutput2.setValue(2048);
      delay(red);
      AnalogOutput2.setValue(0);
      digitalWrite(ledPin, LOW);    // set the LED off
      delay(red);
      AnalogOutput2.setValue(2048);
      delay(red);



 // }//fin muda frecuencia
  //red = 500;

}


