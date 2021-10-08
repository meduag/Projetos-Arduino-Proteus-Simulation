#include <LiquidCrystal.h> //importando biblioteca de LiquidCrustal

LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);

#define START_SYNC_PIN 53
#define SYNC_PIN 48
#define ACCEL_PIN A15

int ctrl = 0;
int valor = 0;

void setup() {

  lcd.begin(16, 2);
  //Serial.begin(128000); // PLX-DAQ excel
  Serial.begin(115200); // PLX-DAQ excel
  //Serial.println("CLEARDATA");
  //Serial.println("LABEL,c_time,valor");

  //Pino sel sensor de accel
  pinMode(ACCEL_PIN, INPUT);

  //Habilitaçao gravacion de dados
  pinMode(START_SYNC_PIN, OUTPUT);
  digitalWrite(START_SYNC_PIN, LOW);

  lcd.clear();
}

void loop() {

  if (ctrl == 0) {
    lcd.setCursor(0, 0);
    lcd.print("Waiting     ");
    lcd.setCursor(0, 1);
    lcd.print("            ");
  }

  //while (digitalRead(SYNC_PIN)) {
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("    3   ");
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("    2   ");
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("    1   ");
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("    0   ");

  int t0 = millis();

  while (millis() - t0 < 10000) {
    /*if (ctrl == 0) {
      lcd.setCursor(0, 0);
      lcd.print("Inicio PLX   ");
      lcd.setCursor(0, 1);
      lcd.print(valor);
      ctrl = 1;
      digitalWrite(START_SYNC_PIN, HIGH);
    }*/

    valor = analogRead(ACCEL_PIN);
    //Serial.print("DATA,TIME,");
    Serial.println(valor);

    //delayMicroseconds(1);
  }

  if (ctrl == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Fin         ");
    digitalWrite(START_SYNC_PIN, LOW);
    ctrl = 0;
    delay(10000);
  }

}


