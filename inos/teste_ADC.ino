#include <LiquidCrystal.h> //importando biblioteca de LiquidCrustal

LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7); //config pines en los que se conecta

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
}

void loop() {
  // put your main code here, to run repeatedly:
  int valor = analogRead(7);
  valor = map(valor, 0, 1024, 0, 500);

  lcd.setCursor(0, 0);
  lcd.print(valor);
  
}
