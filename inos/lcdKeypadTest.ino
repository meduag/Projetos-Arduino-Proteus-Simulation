#include <LiquidCrystal.h>

// select the pins used on the LCD panel
LiquidCrystal lcd(7, 6, 11, 10, 9, 8);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
//int adc_key_in2  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int res     = 0;


/////////////////////////////////////////////////////////////************************************************************
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
  lcd.begin(16, 2);  
  lcd.setCursor(0,0);
  lcd.print("WELCOME!"); 
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("          ");
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0,0);
  lcd.print(ReadValueKeypad());
  delay(100);
  lcd.setCursor(0,1);
  lcd.print("        ");
  lcd.setCursor(0,1);
  lcd.print(analogRead(11));
  delay(100);
  

}
