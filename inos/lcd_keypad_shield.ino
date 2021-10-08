
#include <LiquidCrystal.h> //importando biblioteca de LiquidCrustal
LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7); //pines en los que se conecta

int sensorPin = A0;
int botonA0 = 0;

byte cc = 0;//Variable Byte alto
byte L = 0;//Variable Byte bajo

int c = 1;



void setup()
{
  Serial.begin(9600);
  Serial.println("Hola");
  lcd.begin(16, 2);
  lcd.print("Teste Estevao");
  delay(1000);
}

void loop() {
  //botonA0 = analogRead(sensorPin);

  lcd.setCursor(0, 2);
  lcd.print("Contando");
  lcd.setCursor(9, 2);
  cc = lowByte(c);
  L = Serial.println(cc,BIN);
  Serial.print("Dos ");
  Serial.println(L);
  lcd.print(cc,BIN);
  delay(1000);

  if (c == 59) {
    c = 1;
  } else {
    c = c + 1;
  }

}
