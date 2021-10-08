#include <LiquidCrystal.h> //importando biblioteca de LiquidCrustal
#include <Wire.h>//importando biblioteca para com I2C

LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7); //config pines en los que se conecta

int lcd_key     = 0;       // define os valores representativos de cada botão
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5



const int ledPin = 13;
char mensaje;//INICIAMOS UNA CADENA
String state = "Nada";
int key_stado = 0;

void setup()
{
  Wire.begin();//INICIAMOS EL WIRE (I2C)
  Serial.begin(9600);// INICIAMOS EL SERIAL PARA MOSTRAR LOS MENSAJES
  Serial.println("Maestro I2c");//IMPRIMIMOS DE QUE HEMOS PEDIDO EL MESNAJE AL DISPOSITIVO EN EL CANAL 1
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Iniciando");
  lcd.setCursor(0, 1);
  lcd.print("Mestro I2C");
  delay(1000);
  lcd.clear();
}

void loop() {

  lcd.setCursor(0, 0);
  lcd.print("Mestro I2C");

  lcd_key = read_LCD_buttons();  // Lê os botões
  delay(100);

  if (lcd_key == 0 ) {
    lcd.setCursor(0, 1);
    lcd.print("Canal 1");
    Wire.requestFrom(1, 16); //REALIZAMOS UNA PETICION AL CANAL 1 DE 16 CARACTERES (BITS)
    Serial.println("\nMensaje pedido canal 1");//IMPRIMIMOS DE QUE HEMOS PEDIDO EL MESNAJE AL DISPOSITIVO EN EL CANAL 1

  } else   if (lcd_key == 3 ) {
    lcd.setCursor(0, 1);
    lcd.print("Canal 2");
    Wire.requestFrom(2, 16); //REALIZAMOS UNA PETICION AL CANAL 2 DE 16 CARACTERES (BITS)
    Serial.println("\nMensaje pedido canal 2");//IMPRIMIMOS DE QUE HEMOS PEDIDO EL MESNAJE AL DISPOSITIVO EN EL CANAL 1
  }


  while (Wire.available())
  {
    mensaje = Wire.read();//GUARDAMOS EL MENSAJE EN LA CADENA MESAJE 1
    Serial.print(mensaje);//IMPRIMIMOS EL MENSAJE
    delay(50);
  }

  delay(500);

  if (lcd_key == 0 ) {
    lcd.setCursor(0, 0);
    lcd.print("Canal 1");
    lcd.setCursor(0, 1);
    lcd.print(mensaje);
  } else   if (lcd_key == 3 ) {
    lcd.setCursor(0, 0);
    lcd.print("Canal 2");
    lcd.setCursor(0, 1);
    lcd.print(mensaje);
  }else{
     lcd.clear();
  }
  



}


//Para capturar los botnes del Keypad de la LCD
int read_LCD_buttons()   // função para ler os valores
{
  adc_key_in = analogRead(0);
  if (adc_key_in > 1000) {
    return btnNONE;
    state = "Nada";
    key_stado = 5;
  }

  else if (adc_key_in < 50) {
    return btnRIGHT;
    state = "RIGHT";
    key_stado = 0;
  }

  else  if (adc_key_in < 250) {
    return btnUP;
    state = "UP";
    key_stado = 1;
  }

  else if (adc_key_in < 450) {
    return btnDOWN;
    state = "DOWN";
    key_stado = 2;
  }

  else  if (adc_key_in < 650) {
    return btnLEFT;
    state = "LEFT";
    key_stado = 3;
  }

  else if (adc_key_in < 850) {
    return btnSELECT;
    state = "SELECT";
    key_stado = 4;
  }

  else {
    return btnNONE;  // when all others fail, return this...
    state = "Ninguen B opr";
    key_stado = 5;
  }
}
