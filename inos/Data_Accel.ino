#include <LiquidCrystal.h> //importando biblioteca de LiquidCrustal

LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);

#define ACCEL_PIN_Z A15 // pino de adquisicion de dato eje Z = A15, X = A14
#define ACCEL_PIN_Y A14 // pino de adquisicion de dato eje Z = A15, X = A14
#define ACCEL_PIN_X A13 // pino de adquisicion de dato eje Z = A15, X = A14

#define SYNC_PIN  52 // //pino que inicia la sincronizacion de datos
#define STIM_PIN_IN 48 // pino de iniocio y final del pulso


int ctrl = 0;
int valor_Z = 0;
int valor_Y = 0;
int valor_X = 0;
int stim_sync = 0;


void setup() {

  lcd.begin(16, 2); // Inicializa la LCD
  Serial.begin(115200); // 2KHz adquicicion 2500 a 2300 amostras
  pinMode(ACCEL_PIN_Z, INPUT);//confg pin analogico, como entrada
  pinMode(ACCEL_PIN_X, INPUT);//confg pin analogico, como entrada

  pinMode(SYNC_PIN, INPUT);//confg pin digital, como entrada
  pinMode(STIM_PIN_IN, INPUT);//confg pin analogico, como entrada

  lcd.clear();
  pinMode(ACCEL_PIN_X,INPUT);
  pinMode(ACCEL_PIN_Y,INPUT);
  pinMode(ACCEL_PIN_Z,INPUT);
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
    valor_Y = analogRead(ACCEL_PIN_Y);
    valor_X = analogRead(ACCEL_PIN_X);
    stim_sync = digitalRead(STIM_PIN_IN);

    Serial.print(stim_sync);
    Serial.print(";");
    Serial.print(valor_X);
    Serial.print(";");
    Serial.print(valor_Y);
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


