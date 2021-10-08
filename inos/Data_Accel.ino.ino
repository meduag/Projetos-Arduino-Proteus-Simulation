#include <LiquidCrystal.h> //importando biblioteca de LiquidCrustal

LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);

//#define START_SYNC_PIN 32 //confirmacion visual led
#define SYNC_PIN  52 // //pino que inicia la sincronizacion de datos
#define ACCEL_PIN_Z A15 // pino de adquisicion de dato eje Z = A15, X = A14
#define ACCEL_PIN_X A14 // pino de adquisicion de dato eje Z = A15, X = A14


int ctrl = 0;
int valor_Z = 0;
int valor_X = 0;


void setup() {

  lcd.begin(16, 2); // Inicializa la LCD
  Serial.begin(115200); // 2KHz adquicicion 2500 a 2300 amostras
  pinMode(ACCEL_PIN_Z, INPUT);//confg pin analogico, como entrada
  pinMode(ACCEL_PIN_X, INPUT);//confg pin analogico, como entrada

  pinMode(SYNC_PIN, INPUT);//confg pin digital, como entrada

  //Habilitaçao gravacion de dados
  //pinMode(START_SYNC_PIN, OUTPUT);
  //digitalWrite(START_SYNC_PIN, LOW);

  lcd.clear();


}

void loop() {

  //esperando iniciacion do Estimulador
  if (ctrl == 0) {
    lcd.setCursor(0, 0);
    lcd.print("Waiting     ");
    lcd.setCursor(0, 1);
    lcd.print("            ");
  }


  //envia dados para gravacion
  while (digitalRead(SYNC_PIN)) {
    if (ctrl == 0) {
      lcd.setCursor(0, 0);
      lcd.print("Inicio      ");
      //lcd.setCursor(0, 1);
      //lcd.print(valor_Z);
      ctrl = 1;
      //digitalWrite(START_SYNC_PIN, HIGH);
    }

    valor_Z = analogRead(ACCEL_PIN_Z);
    valor_X = analogRead(ACCEL_PIN_X);
    Serial.print(valor_X);
    Serial.print(";");
    Serial.println(valor_Z);

  }

  if (ctrl == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Fin         ");
    //digitalWrite(START_SYNC_PIN, LOW);
    ctrl = 0;
    delay(10000);
  }

}


