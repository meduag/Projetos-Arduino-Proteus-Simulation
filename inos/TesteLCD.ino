#include <LiquidCrystal.h> //importando biblioteca de LiquidCrustal

LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7); //config pines en los que se conecta

void setup() {
  pinMode(INPUT, A8);
  pinMode(INPUT, A9);
  pinMode(INPUT, A10);
  pinMode(INPUT, A11);

  lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.clear();
}

void loop() {

  int V1 = analogRead(A8);
  int V2 = analogRead(A9);
  int V3 = analogRead(A10);
  int V4 = analogRead(A11);

  delay(200);
  //lcd.clear();

  /*  Serial.println("-------------------------------------------");

    Serial.print("S1: ");
    Serial.print(V1);
    Serial.print(" - ");

    Serial.print("S2: ");
    Serial.print(V2);
    Serial.print(" - ");

    Serial.print("S3: ");
    Serial.print(V3);
    Serial.print(" - ");

    Serial.print("S4: ");
    Serial.println(V4); */

  V1 = 20;
  V2 = 21;
  V3 = 22; 
  V4 = 23,;

  int Vt1 = map (V1, 0, 1023, 0, 100);
  int Vt2 = map (V2, 0, 1023, 0, 100);
  int Vt3 = map (V3, 0, 1023, 0, 100);
  int Vt4 = map (V4, 0, 1023, 0, 100);

  Serial.print(Vt1);
  Serial.print(";");
  Serial.print(Vt2);
  Serial.print(";");
  Serial.print(Vt3);
  Serial.print(";");
  Serial.println(Vt4);

  //Para mostrar en la LCD
  lcd.setCursor(0, 0);
  lcd.print("S1 ");
  lcd.print(Vt1);

  lcd.setCursor(8, 0);
  lcd.print("S2 ");
  lcd.print(Vt2);

  lcd.setCursor(0, 1);
  lcd.print("S3 ");
  lcd.print(Vt3);

  lcd.setCursor(8, 1);
  lcd.print("S4 ");
  lcd.print(Vt4);

}
