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
char mensaje;
int key_stado = 0;
char dado ;

int T1 = 0, T2 = 0, T3 = 0, T4 = 0, mA = 0, Fq = 0;

char* Menu_WFQ[] = {"Tempo 1 uS", "Tempo 2 uS", "Tempo 3 uS",
                    "Tempo 4 uS", "Corrente mA", "Freq Hz"};


void setup()
{
  Wire.begin();//INICIAMOS EL WIRE (I2C)
  Serial.begin(9600);// INICIAMOS EL SERIAL PARA MOSTRAR LOS MENSAJES
  Serial.println("Prototipo 1 Iniciando");//IMPRIMIMOS DE QUE HEMOS PEDIDO EL MESNAJE AL DISPOSITIVO EN EL CANAL 1
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Iniciando");
  lcd.setCursor(0, 1);
  lcd.print("Protótipo 1");

  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("Iniciando..");
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("Iniciando....");
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("Iniciando......");
  delay(500);


  lcd.clear();
}

void loop() {
  Menu_WFQLCD();


}//fin void loop


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

void Menu_WFQLCD() {
  int sel = 0;
  lcd.setCursor(0, 0);
  lcd.print("Menu_WFQ        ");

  lcd_key = 0;
  lcd_key = read_LCD_buttons();  // Lê os botões
  delay(100);

  switch (lcd_key)// Para cada botão escolhido, uma ação acontece
  {
    case btnRIGHT:
      {
        lcd.setCursor(0, 1);
        lcd.print(Menu_WFQ[sel]);
        lcd_key = 0;
        lcd_key = read_LCD_buttons();  // Lê os botões
        delay(100);

        sel += 1;
        break;
      }
    case btnLEFT:
      {
        lcd.setCursor(0, 1);
        lcd.print("Left ");
        break;
      }
    case btnUP:
      {
        lcd.setCursor(0, 1);
        lcd.print("Up ");
        break;
      }
    case btnDOWN:
      {
        lcd.setCursor(0, 1);
        lcd.print("Down ");
        break;
      }
    case btnSELECT:
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(Menu_WFQ[sel]);
        MudarValor(Menu_WFQ[sel]);
        break;
      }
    default:
      break;

  }


}


void MudarValor(String Mv) {
  String WFQ0 = Menu_WFQ[0];
  String WFQ1 = Menu_WFQ[1];
  String WFQ2 = Menu_WFQ[2];
  String WFQ3 = Menu_WFQ[3];
  String WFQ4 = Menu_WFQ[4];
  String WFQ5 = Menu_WFQ[5];

  int entrar = 0;


  if (WFQ0.equals(Mv)) {
    lcd.setCursor(0, 1);
    lcd.print(Mv);

    lcd.setCursor(0, 1);
    lcd.print("T1 = ");
    lcd.setCursor(6, 1);
    lcd.print(T1);
    lcd.print(" uS");
    lcd_key = 0;

    do {
      lcd_key = read_LCD_buttons();  // Lê os botões
      delay(100);
      if (lcd_key == 1) {
        T1 = T1 + 1;
        lcd.setCursor(6, 1);
        lcd.print(T1);
        lcd.print(" uS");
      }
      if (lcd_key == 2 && T1 > 0) {
        T1 = T1 - 1;
        lcd.setCursor(6, 1);
        lcd.print(T1);
        lcd.print(" uS");
      }
      if (lcd_key == 4) {
        entrar = 1;
      } else {
        lcd_key = 0;
      }
    } while (entrar == 0);
  }

  if (WFQ1.equals(Mv)) {
    lcd.setCursor(0, 1);
    lcd.print("T2 = ");
    lcd.setCursor(6, 1);
    lcd.print(T2);
    lcd.print(" uS");
	
	lcd_key = 0;

    do {
      lcd_key = read_LCD_buttons();  // Lê os botões
      delay(100);
      if (lcd_key == 1) {
        T2 = T2 + 1;
        lcd.setCursor(6, 1);
        lcd.print(T2);
        lcd.print(" uS");
      }
      if (lcd_key == 2 && T2 > 0) {
        T2 = T2 - 1;
        lcd.setCursor(6, 1);
        lcd.print(T2);
        lcd.print(" uS");
      }
      if (lcd_key == 4) {
        entrar = 1;
      } else {
        lcd_key = 0;
      }
    } while (entrar == 0);
  }

  if (WFQ2.equals(Mv)) {
    lcd.setCursor(0, 1);
    lcd.print("T3 = ");
    lcd.setCursor(6, 1);
    lcd.print(T3);
    lcd.print(" uS");

	lcd_key = 0;

    do {
      lcd_key = read_LCD_buttons();  // Lê os botões
      delay(100);
      if (lcd_key == 1) {
        T3 = T3 + 1;
        lcd.setCursor(6, 1);
        lcd.print(T3);
        lcd.print(" uS");
      }
      if (lcd_key == 2 && T3 > 0) {
        T3 = T3 - 1;
        lcd.setCursor(6, 1);
        lcd.print(T1);
        lcd.print(" uS");
      }
      if (lcd_key == 4) {
        entrar = 1;
      } else {
        lcd_key = 0;
      }
    } while (entrar == 0);
  }

  if (WFQ3.equals(Mv)) {
    lcd.setCursor(0, 1);
    lcd.print("T4 = ");
    lcd.setCursor(6, 1);
    lcd.print(T4);
    lcd.print(" uS");
    lcd_key = 0;

    do {
      lcd_key = read_LCD_buttons();  // Lê os botões
      delay(100);
      if (lcd_key == 1) {
        T4 = T4 + 1;
        lcd.setCursor(6, 1);
        lcd.print(T4);
        lcd.print(" uS");
      }
      if (lcd_key == 2 && T4 > 0) {
        T4 = T4 - 1;
        lcd.setCursor(6, 1);
        lcd.print(T1);
        lcd.print(" uS");
      }
      if (lcd_key == 4) {
        entrar = 1;
      } else {
        lcd_key = 0;
      }
    } while (entrar == 0);
  }

  if (WFQ4.equals(Mv)) {
    lcd.setCursor(0, 1);
    lcd.print("A = ");
    lcd.setCursor(5, 1);
    lcd.print(mA);
    lcd.print(" mA");
	
	lcd_key = 0;

    do {
      lcd_key = read_LCD_buttons();  // Lê os botões
      delay(100);
            mA = mA + 1;
      if (lcd_key == 1) {
        lcd.setCursor(6, 1);
            lcd.print(mA);
    lcd.print(" mA");
      }
      if (lcd_key == 2 && mA > 0) {
        mA = mA - 1;
        lcd.setCursor(6, 1);
            lcd.print(mA);
    lcd.print(" mA");
      }
      if (lcd_key == 4) {
        entrar = 1;
      } else {
        lcd_key = 0;
      }
    } while (entrar == 0);
  }
  if (WFQ5.equals(Mv)) {
    lcd.setCursor(0, 1);
    lcd.print("Fq = ");
    lcd.setCursor(6, 1);
    lcd.print(Fq);
    lcd.print(" Hz");
	
	lcd_key = 0;

    do {
      lcd_key = read_LCD_buttons();  // Lê os botões
      delay(100);
                Fq = Fq + 1;
      if (lcd_key == 1) {
        lcd.setCursor(6, 1);
            lcd.print(mA);
    lcd.print(" mA");
      }
      if (lcd_key == 2 && Fq > 0) {
            Fq = Fq - 1;
        lcd.setCursor(6, 1);
            lcd.print(Fq);
    lcd.print(" Fq");
      }
      if (lcd_key == 4) {
        entrar = 1;
      } else {
        lcd_key = 0;
      }
    } while (entrar == 0);
  }

}


/*

  lcd.setCursor(0, 0);
  lcd.print("M I2C W_DAC");
  lcd_key = 0;
  lcd_key = read_LCD_buttons();  // Lê os botões
  delay(100);

  if (lcd_key == 0 ) {
    lcd.setCursor(0, 1);
    lcd.print("Canal 1");
    Wire.beginTransmission(1);
    Wire.write("Enviando canal 1");//Imprimimos cuando se realize la peticion ese mensaje y lo enviaremos por el canal 1
    Wire.endTransmission();
    Serial.println("Dato enviado al canal 1");//IMPRIMIMOS DE QUE HEMOS PEDIDO EL MESNAJE AL DISPOSITIVO EN EL CANAL 1
  } else   if (lcd_key == 3 ) {
    lcd.setCursor(0, 1);
    lcd.print("Canal 2");

    Wire.beginTransmission(2);
    Wire.write("enviando");
    Wire.write(" End_T");
    Wire.endTransmission();

    Serial.println("Dato enviado al canal 2");//IMPRIMIMOS DE QUE HEMOS PEDIDO EL MESNAJE AL DISPOSITIVO EN EL CANAL 1
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
  } else {
    lcd.clear();
  }
*/
