#include "AH_MCP4922.h"

//define AnalogOutput (MOSI_pin, SCK_pin, CS_pin, DAC_x, GAIN)
AH_MCP4922 AnalogOutput1(21, 22, 23, LOW, HIGH); //define AnalogOutput1 for MEGA_board, select DAC_A, Gain=1x
AH_MCP4922 AnalogOutput2(21, 22, 23, HIGH, HIGH); //define AnalogOutput2 for MEGA_board, select DAC_B, Gain=2x

int T1 = 100, T2 = 100, Cor = 4095;

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(13, HIGH);
  pasardatoDAC(3, 4095, 10, 20);
  digitalWrite(13, LOW);
  /* pasardatoDAC(1, 4095, 10, 20);
   digitalWrite(13, HIGH);
   pasardatoDAC(2, 4095, 10, 20);
   digitalWrite(13, LOW);*/

  delay(2000);

  AnalogOutput1.setValue(Cor - 200);
  AnalogOutput2.setValue(Cor - 200);
  delay(T1);
  AnalogOutput1.setValue(Cor - 200);
  AnalogOutput2.setValue(Cor - 200);
  delay(T1);
  AnalogOutput1.setValue(2048);
  AnalogOutput2.setValue(2048);
  delay(T2);
}

///////////////////////////////////
void pasardatoDAC(int DAC, int Cor, int T1, int T2) {
  if ( DAC == 3 ) {
    AnalogOutput1.setValue(Cor);
    AnalogOutput2.setValue(Cor);
    delay(T1);
    AnalogOutput1.setValue(4095-Cor);
    AnalogOutput2.setValue(4095-Cor);
    delay(T1);
    AnalogOutput1.setValue(2048);
    AnalogOutput2.setValue(2048);
    delay(T2);
  } else if ( DAC == 1 ) {
    AnalogOutput1.setValue(Cor);
    delay(T1);
    AnalogOutput1.setValue(4095-Cor);
    delay(T1);
    AnalogOutput1.setValue(2048);
    delay(T2);
  } else {
    AnalogOutput2.setValue(Cor);
    delay(T1);
    AnalogOutput2.setValue(4095-Cor);
    delay(T1);
    AnalogOutput2.setValue(2048);
    delay(T2);
  }
  
  piscaLed(40);

}

void piscaLed(int T1){
  digitalWrite(13, HIGH);
  delay(T1);
  digitalWrite(13, LOW);
  delay(T1);
  digitalWrite(13, HIGH);
  delay(T1);
  digitalWrite(13, LOW);
  delay(T1);  
}

