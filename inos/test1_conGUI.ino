#include <LiquidCrystal.h>
#include "AH_MCP4922.h"

// select the pins used on the LCD panel
LiquidCrystal lcd(7, 6, 11, 10, 9, 8);

//define AnalogOutput (MOSI_pin, SCK_pin, CS_pin, DAC_x, GAIN)
AH_MCP4922 AnalogOutput1(21, 22, 23, LOW, HIGH); //define AnalogOutput1 for MEGA_board, select DAC_A, Gain=1x
AH_MCP4922 AnalogOutput2(21, 22, 23, HIGH, HIGH); //define AnalogOutput2 for MEGA_board, select DAC_B, Gain=2x

////////////////////////////////////////////////Variaveis
// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int T1 = 0, T2 = 0, T3 = 0, Cor = 0, DAC = 0, SentDado = 0;

char entrada = 'D';
//char dado = '0';

//Declaracion de variables
char inChar;
String dado = "";
String temp = "";
char enviar = 'Y';//puede ser Yes o Not


/////////////////////////////////////////////////////////////FunÃ§oes
int ReadValueKeypad()
{
  adc_key_in = analogRead(11);      // lÃª os valores do sensor
  // valores lidos de cada botÃ£o: 0, 144, 329, 504, 741
  if (adc_key_in > 1000) return btnNONE; // faixa de valores para cada botÃ£o
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 250)  return btnUP;
  if (adc_key_in < 450)  return btnDOWN;
  if (adc_key_in < 650)  return btnLEFT;
  if (adc_key_in < 850)  return btnSELECT;
  return btnNONE;  //se todos falharem, retorna esse
}


void setup() {
  //configuracion de porta
  Serial.begin(9600); //Open the serial port
  dado.reserve(200);//reserva memoria en el dispositivo
  temp.reserve(200);//reserva memoria en el dispositivo

  pinMode(13, OUTPUT);

  piscaLed(30);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Esperando");
  lcd.setCursor(0, 1);
  lcd.print("Conexion");
  delay(1000);

}

void loop() {

  if (Serial.available() > 0) {

    dado = RecibirDados();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Recibiendo");
    /*delay(1000);
    lcd.setCursor(0, 0);
    lcd.print("Decompor");
    lcd.setCursor(0, 1);
    lcd.print("                ");//limpia la linea
    delay(1000);
    lcd.setCursor(0, 0);
    lcd.print("Decompor dados");*/

    /*    for (int i = 0; i < 20; i++) {

             lcd.setCursor(0, 1);
             lcd.print(dado[i]);
             delay(200);



          if (i == 3) {
            lcd.setCursor(0, 1);
            //lcd.print("                ");//limpia la linea
            lcd.print(temp + " -");
            delay(1000);
            temp = "";
          }

          if (i == 6) {
            lcd.setCursor(0, 1);
            lcd.print(temp + " -");
            delay(1000);
            temp = "";
          } else {
            temp = temp + dado[i];
          }


        }*/

    SentDado = ExtraerDados(dado, 4);
    /* lcd.setCursor(0, 0);
     lcd.print("Confirmacion: ");
     lcd.print(SentDado);
     lcd.setCursor(0, 1);
     lcd.print("                ");//limpia la linea
     piscaLed(200);*/

    int vp = 1;

    if (SentDado == 1) {
      DAC = ExtraerDados(dado, 0);
      /*      lcd.setCursor(0, 1);
            lcd.print("                ");//limpia la linea
            lcd.setCursor(0, 1);
            lcd.print("DAC No ");
            lcd.print(DAC);
            piscaLed(vp);*/

      Cor = ExtraerDados(dado, 1);
      /*      lcd.setCursor(0, 1);
            lcd.print("                ");//limpia la linea
            lcd.setCursor(0, 1);
            lcd.print("VCor " );
            lcd.print( Cor);
            piscaLed(vp);*/

      T1 = ExtraerDados(dado, 2);
      /*      lcd.setCursor(0, 1);
            lcd.print("                ");//limpia la linea
            lcd.setCursor(0, 1);
            lcd.print("T1 ");
            lcd.print( T1);
            piscaLed(vp);*/

      T2 = ExtraerDados(dado, 3);
      /*      lcd.setCursor(0, 1);
            lcd.print("                ");//limpia la linea
            lcd.setCursor(0, 1);
            lcd.print("T2 ");
            lcd.print( T2);
            piscaLed(vp);*/
    }

    /*   lcd.setCursor(0, 1);
       lcd.print(DAC);
       lcd.print(Cor);
       lcd.print(T1);
       lcd.print(T2);*/



  }//fin recibir datos

  //digitalWrite(13, HIGH);

  if (SentDado == 1) {
    //piscaLed(40);
    pasardatoDAC(DAC, Cor, T1, T2);
  }

  LimpiarVariables();//limpio las variables para revivir de nuevo


  //pasardatoDAC(DAC, Cor, T1, T2);
  //pasardatoDAC(2,4095,100,200);
  //digitalWrite(13, LOW);
}

///////////////////////////////////
void pasardatoDAC(int DAC, int Cor, int T1, int T2) {
  if ( DAC == 3 ) {
    AnalogOutput1.setValue(Cor);
    AnalogOutput2.setValue(Cor);
    delay(T1);
    AnalogOutput1.setValue(4095 - Cor);
    AnalogOutput2.setValue(4095 - Cor);
    delay(T1);
    AnalogOutput1.setValue(2048);
    AnalogOutput2.setValue(2048);
    delay(T2);
  } else if ( DAC == 1 ) {
    AnalogOutput1.setValue(Cor);
    delay(T1);
    AnalogOutput1.setValue(4095 - Cor);
    delay(T1);
    AnalogOutput1.setValue(2048);
    delay(T2);
  } else {
    AnalogOutput2.setValue(Cor);
    delay(T1);
    AnalogOutput2.setValue(4095 - Cor);
    delay(T1);
    AnalogOutput2.setValue(2048);
    delay(T2);
  }
}

///////////////////////////////////
int ExtraerDados(String mensage, int Val) {
  int Vfin = 0;

  switch (Val) {
    case 0:
      //do something when var equals 0 para o DAC
      Vfin = dadoIndividual(mensage, 1, 2);
      /*lcd.setCursor(0, 1);
      lcd.print("D  " + Vfin);
      piscaLed(40);*/
      break;
    case 1:
      //do something when var equals 1 para Corriente
      /*  lcd.setCursor(0, 1);
        lcd.print("I  " + dadoIndividual(mensage, 6, 9));*/
      Vfin = dadoIndividual(mensage, 4, 7);
      //piscaLed(40);
      break;
    case 2:
      //do something when var equals 2 para T1
      /*  lcd.setCursor(0, 1);
        lcd.print("T1 " + dadoIndividual(mensage, 12, 15));*/
      Vfin = dadoIndividual(mensage, 9, 12);
      //piscaLed(40);
      break;
    case 3:
      //do something when var equals 3 para T2
      /* lcd.setCursor(0, 1);
       lcd.print("T2 " + dadoIndividual(mensage, 18, 21));*/
      Vfin = dadoIndividual(mensage, 14, 17);
      piscaLed(40);
      break;
    case 4:
      //do something when var equals 4 auxiliar
      /*  lcd.setCursor(0, 1);
        lcd.print("M  " + dadoIndividual(mensage, 23, 24)); para saver si tiene que enviar o no el dato*/
      Vfin = dadoIndividual(mensage, 19, 19);
      //piscaLed(40);
      break;
      //default:
      // if nothing else matches, do the default
      // default is optional
  }

  return Vfin;
}


int dadoIndividual(String mensaje, int ini, int fin) {
  int valor = 0;
  temp = "";
  for (int i = ini; i < fin + 1; i++) {
    temp = temp + mensaje[i];
  }
  valor = temp.toInt();
  return valor;
}


///////////////////////////////////
String RecibirDados() {
  while (entrada != '+') {
    entrada = Serial.read();
    if (entrada == '-' || entrada == '+') {
      if (entrada == '-') {
        // lcd.setCursor(0, 0);
        // lcd.print("Recibiendo      ");
        dado = "";
        // delay(100);
      } else {//para terminar (podria ser eliminado)
        //  lcd.setCursor(0, 0);
        //  lcd.print("Fin             ");
        //  delay(100);
      }
    } else {
      dado = dado + entrada;
      // lcd.setCursor(0, 1);
      // lcd.print(entrada);
      //delay(200);
    }

  }
  /*  lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(dado); */
  return dado;
}


void piscaLed(int dd) {
  digitalWrite(13, HIGH);
  delay(dd);
  digitalWrite(13, LOW);
  delay(dd);
  digitalWrite(13, HIGH);
  delay(dd);
  digitalWrite(13, LOW);
  delay(dd);
}

void LimpiarVariables() {
  /*T1 = 0;
  T2 = 0;
  T3 = 0;
  Cor = 0;
  DAC = 0;
  SentDado = 0;*/
  dado = "";
  temp = "";
  entrada = '/';
}








