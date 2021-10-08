//Bibliotecas
#include "AH_MCP4922.h"
#include <LiquidCrystal.h>
#include "stdlib.h"
#include "math.h"

////////////////////////////////////////////////configuraçoes das bibliotecas

// select the pins used on the DAC
AH_MCP4922 AnalogOutput1(21, 22, 23, HIGH, HIGH); //define AnalogOutput1 for MEGA_board, select DAC_A, Gain=1x High
AH_MCP4922 AnalogOutput2(21, 22, 23, LOW, HIGH); //define AnalogOutput2 for MEGA_board, select DAC_B, Gain=2x Low

// select the pins used on the LCD panel
LiquidCrystal lcd(7, 6, 11, 10, 9, 8);

////////////////////////////////////////////////Variaveis
// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

//const int ledPin = 13;
int retardo =1;
int res     = 0;
//int ccor = -400;



/////////////////////////////////////////////////////////////Funçoes
int ReadValueKeypad()
{
 adc_key_in = analogRead(11);      // lê os valores do sensor  
 // valores lidos de cada botão: 0, 144, 329, 504, 741
 if (adc_key_in > 1000 - res) return btnNONE; // faixa de valores para cada botão
 if (adc_key_in < 50 - res)   return btnRIGHT;  
 if (adc_key_in < 250 - res)  return btnUP; 
 if (adc_key_in < 450 - res)  return btnDOWN; 
 if (adc_key_in < 650 - res)  return btnLEFT; 
 if (adc_key_in < 850 - res)  return btnSELECT;  
 return btnNONE;  //se todos falharem, retorna esse
}


void setup() {
  //pinMode(ledPin, OUTPUT);
  lcd.begin(16, 2);  
  lcd.setCursor(0,0);
  lcd.print("WELCOME!"); 
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("          ");
}

//*****************************************************************
void loop() {

      AnalogOutput1.setValue(4095);
      AnalogOutput2.setValue(4095);
      lcd.setCursor(0,0);
      lcd.print("4095");
      lcd.setCursor(0,1);
      lcd.print("    ");
      lcd.setCursor(0,1);
      lcd.print(analogRead(11));
   
      
      //digitalWrite(ledPin, HIGH);   // set the LED on
      delay(retardo);
      AnalogOutput1.setValue(2048);
      AnalogOutput2.setValue(2048);
      lcd.setCursor(0,0);
      lcd.print("2048");
      lcd.setCursor(0,1);
      lcd.print("    ");
      lcd.setCursor(0,1);
      lcd.print(analogRead(11)); 
      
      delay(retardo);
      //digitalWrite(ledPin, LOW);   // set the LED on
      AnalogOutput1.setValue(0);
      AnalogOutput2.setValue(0);
      lcd.setCursor(0,0);
      lcd.print("0   ");
      lcd.setCursor(0,1);
      lcd.print("    ");
      lcd.setCursor(0,1);
      lcd.print(analogRead(11));
      
      //digitalWrite(ledPin, HIGH);   // set the LED on
      delay(retardo);
      AnalogOutput1.setValue(2048);
      AnalogOutput2.setValue(2048);
      lcd.setCursor(0,0);
      lcd.print("2048");
      lcd.setCursor(0,1);
      lcd.print("    ");
      lcd.setCursor(0,1);
      lcd.print(analogRead(11));
      
      //digitalWrite(ledPin, LOW);   // set the LED on
      delay(retardo*2);

}


