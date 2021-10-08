#include <LiquidCrystal.h> //importando biblioteca de LiquidCrustal
#include <EEPROM.h>

LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7); //config pines en los que se conecta

float valt = 0;
int t1 = 0;
String StrVal = "";

int addr = 0;    // endereço de escrita na memória

int lcd_key     = 0;       // define os valores representativos de cada botão
int adc_key_in  = 0;

int buzer = 21;

int minu = 1;//contador de minutos

int mint = 2;//minutos totales


#define btnRIGHT  0///del memoria
#define btnUP     1//comenzar
#define btnDOWN   2
#define btnLEFT   3//para parar la captura
#define btnSELECT 4 /// para pasar los dados
#define btnNONE   5

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  Serial.begin(9600);// INICIAMOS EL SERIAL PARA MOSTRAR LOS MENSAJES
}

void loop() {

  lcd_key = read_LCD_buttons();  // Lê os botões
  delay(100);


  lcd.setCursor(0, 0);
  lcd.print("Esperando   ");

  //Murestra el boton apretado
  Show_button();

  if (lcd_key == 0) {//se usa RIGHT para reiniciar memoria
    Del_EEPROM();
  }

  if (lcd_key == 4) {//se usa SELECT para pasar los datos
    lcd.setCursor(0, 0);
    lcd.print("Pasando datos  ");
    Send_Data();

  } else if (lcd_key == 1) {//se usa UP para comenzar
    Capture_Data(mint);
  }

}







//Para capturar los botnes del Keypad de la LCD
int read_LCD_buttons()   // função para ler os valores
{
  adc_key_in = analogRead(0);
  if (adc_key_in > 1000) {
    return btnNONE;
  }
  else if (adc_key_in < 50) {
    return btnRIGHT;
  }
  else  if (adc_key_in < 250) {
    return btnUP;
  }
  else if (adc_key_in < 450) {
    return btnDOWN;
  }
  else  if (adc_key_in < 650) {
    return btnLEFT;
  }
  else if (adc_key_in < 850) {
    return btnSELECT;
  }
  else {
    return btnNONE;  // when all others fail, return this...
  }
}




int Show_button() {
  lcd.setCursor(0, 1);
  lcd.print("Boton ");
  lcd.print(lcd_key);
}




int Del_EEPROM()   // função para borrar EEPROM
{
  lcd.setCursor(0, 0);
  lcd.print("Borrando EEPROM");
  for (int address = 0; address <= 4095; address++) {
    EEPROM.write(address, 0);//guardo solo el valor entero
  }
  lcd.clear();

  lcd.print("Pasando EEPROM");
  for (int address = 0; address <= (60 * mint); address++) {
    Serial.print(address);
    Serial.print("  ");
    int value = EEPROM.read(address);
    Serial.println(value);

    lcd.setCursor(0, 1);
    lcd.print("Amostra ");
    lcd.print(address);
    lcd.print(" ");
    lcd.print(value);
  }
  addr = 0;
  lcd.clear();
}





int Send_Data() {
  //leo los valores por
  lcd.setCursor(0, 0);
  lcd.print("Pasando EEPROM");

  Serial.println("Enviando datos de temp ****************************************");
  for (int address = 0; address <= (60 * mint); address++) {
    int value = EEPROM.read(address);
    Serial.println(value);

    lcd.setCursor(0, 1);
    lcd.print("Amostra ");
    lcd.print(address);
    lcd.print(" ");
    lcd.print(value);
  }
  Serial.println("Fin ************************************************");
  lcd.clear();
}




int Capture_Data(int minutos) { //recibe la cantidad de minutos
  minutos = minutos * 60;
  Serial.println("Son ");
  Serial.println(minutos);
  Serial.println(" minutos");

  int tboton = 100;
  int bp = 0;

  lcd.setCursor(0, 0);
  lcd.print("Iniciando");
  delay(tboton * 10);


  while (addr <= minutos) {

    //bip de minutos
    if (addr == (minu * 60)) {
      //beep de minutos
      beep(1);
      minu = minu + 1;
    }


    lcd_key = read_LCD_buttons();  // Lê os botões
    delay(tboton);

    //capturo el valor de la temperatura
    valt = analogRead(15);
    valt = (valt * 0.00488) * 10;//arreglo el valor a grados y lo muestro por la LCD
    lcd.setCursor(0, 0);
    lcd.print("T = ");
    lcd.print(valt);

    //Coloco las amostras
    lcd.print(" ");
    lcd.print(addr);

    //Arreglo el valor para la puerta serial
    t1 = valt;
    EEPROM.write(addr, t1);//guardo solo el valor entero
    StrVal.concat(t1) ;
    StrVal.concat(".") ;
    t1 = (valt - t1) * 100;
    StrVal.concat(t1) ;

    Serial.println(StrVal);
    StrVal = "";//reinicio la variable

    //adiciono os endereços
    addr = addr + 1;

    if (addr == 4095 || addr == minutos) {
      addr = 0;
      beep(0);
      lcd.setCursor(0, 0);
      lcd.print("Finalizando");
      delay(tboton * 10);
      break;
    }

    if (lcd_key == 3) {//para la captura
      addr == minutos + 1;
      beep(0);
      lcd.setCursor(0, 0);
      lcd.print("Finalizando  ");
      delay(tboton * 10);
      break;
    }

    delay(1000 - (tboton * 2));

    if (bp == 1) {
      beep(0);
      bp = 0;
    }


  }//fin while

  lcd.setCursor(0, 0);
  lcd.print("Silio while  ");
  delay(tboton * 10);

}




int beep(int active) {
  if (active == 1) {
    digitalWrite(buzer, HIGH);
  } else {
    digitalWrite(buzer, LOW);
  }

}

