#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int val = 0;
int adc_key_in  = 0;

//valores das variabeis
int I1 = 0, I2 = 0, U1 = 0, U2 = 0;

int c = 1;
int c2 = 1;



void setup() {
  lcd.begin(16, 2);
  delay(2000);
}

void loop() {

  val = read_LCD_buttons();
  if (val != 5) {
    atualizar(val);
  }

  I1 = c;
  U1 = c;
  I2 = c;
  U2 = c;


  //cuadrante 1
  lcd.setCursor(0, 0);
  lcd.print("I");
  lcd.setCursor(2, 0);
  lcd.print(I1);


  //cuadrante 1
  lcd.setCursor(12, 0);
  lcd.print("I");
  lcd.setCursor(14, 0);
  lcd.print(I2);


  //cuadrante 1
  lcd.setCursor(0, 1);
  lcd.print("U"); // print a simple message
  lcd.setCursor(2, 1);
  lcd.print(U1);

  //cuadrante 1
  lcd.setCursor(12, 1);
  lcd.print("U"); // print a simple message
  lcd.setCursor(14, 1);
  lcd.print(U2);

  //delay(1000);

  if (c2 == 59) {
    //c = 1;
    c = c + 1;
    //c = c2;
    c2 = 1;
    lcd.clear();
  } else {
    c2 = c2 + 1;
  }

}

int atualizar(int v) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Boton");
  lcd.setCursor(6, 0);
  lcd.print(v);

  c = 1;
  c2 = 1;

  delay(1000);
  lcd.clear();
}

int read_LCD_buttons()
{
  adc_key_in = analogRead(0);
  if (adc_key_in > 1000) return btnNONE;
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 250)  return btnUP;
  if (adc_key_in < 450)  return btnDOWN;
  if (adc_key_in < 650)  return btnLEFT;
  if (adc_key_in < 850)  return btnSELECT;

  return btnNONE;
}
