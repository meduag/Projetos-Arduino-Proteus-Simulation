#include <LiquidCrystal.h> //importando biblioteca de LiquidCrustal

LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7); //config pines en los que se conecta

int lcd_key     = 0;       // define os valores representativos de cada botão
int adc_key_in  = 0;
int key_stado = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

void setup() {
  Serial.begin(9600);// INICIAMOS EL SERIAL PARA MOSTRAR LOS MENSAJES
  lcd.begin(16, 2);
}




void loop() {
  //colocar los menus o el menú
  // Iniciar_ProtMensa();

  lee_botones();
}//fin void loop


void lee_botones() {
  lcd.clear();
  int in1 = analogRead(A2);
  int in2 = analogRead(A3);
  int in3 = analogRead(A4);

  lcd.setCursor(0, 0);
  lcd.print("A: ");
  lcd.setCursor(3, 0);
  lcd.print(in1);

  lcd.setCursor(8, 0);
  lcd.print("B: ");
  lcd.setCursor(11, 0);
  lcd.print(in2);

  lcd.setCursor(0, 1);
  lcd.print("C: ");
  lcd.setCursor(3, 1);
  lcd.print(in3);
  delay(200);
}

//Para capturar los botnes del Keypad de la LCD
int read_LCD_buttons()   // função para ler os valores
{
  adc_key_in = analogRead(0);
  if (adc_key_in > 1000) {
    return btnNONE;
    //state = "Nada";
    key_stado = 5;
  }

  else if (adc_key_in < 50) {
    return btnRIGHT;
    //state = "RIGHT";
    key_stado = 0;
  }

  else  if (adc_key_in < 250) {
    return btnUP;
    //state = "UP";
    key_stado = 1;
  }

  else if (adc_key_in < 450) {
    return btnDOWN;
    //state = "DOWN";
    key_stado = 2;
  }

  else  if (adc_key_in < 650) {
    return btnLEFT;
    //state = "LEFT";
    key_stado = 3;
  }

  else if (adc_key_in < 850) {
    return btnSELECT;
    //state = "SELECT";
    key_stado = 4;
  }

  else {
    return btnNONE;  // when all others fail, return this...
    //state = "Ninguen B opr";
    //key_stado = 5;
  }
}

void Iniciar_ProtMensa() {
  lcd.setCursor(0, 0);
  lcd.print("  Prototipo V1");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando");
  delay(200);
  puntos(100);
}

void puntos(int dIn) {
  lcd.print(".");
  delay(dIn);
  lcd.print(".");
  delay(dIn);
  lcd.print(".");
  delay(dIn);
  lcd.print(".");
  delay(dIn);
  lcd.print(".");
  delay(dIn);
  lcd.print(".");
  delay(dIn);
  delay(dIn);
  lcd.clear();
}
