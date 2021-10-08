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

int T1=0, T2=0, T3=0;

char entrada = 'inicial';
//char dado = '0';

//Declaracion de variables
char inChar;
String dado = "";
public char enviar = 'Y';//puede ser Yes o Not


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
  dado.reserve(200);

  //pinMode(ledPin, OUTPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Esperando");
  delay(2000);

}

void loop() {
  if (Serial.available() > 0) {
    entrada = Serial.read();

    if (entrada == '-' || entrada == '+') {
      if (entrada == '-') {
        lcd.setCursor(0, 0);
        lcd.print("Recibiendo");
        //lcd.clear();
        dado = "";
        /*lcd.setCursor(0, 0);
        lcd.print("Inicio          ");*/
        delay(200);
      } else {
        /*lcd.setCursor(0, 0);
        lcd.print("Fin             ");
        lcd.setCursor(0, 1);
        lcd.print(dado);*/
      }
    } else {
      dado = dado + entrada;
      /*lcd.setCursor(0, 1);
      lcd.print(entrada);*/
      delay(200);
    }
  }//fin recibir datos

  

}

///////////////////////////////////
void pasardatoDAC(int DAC, int Cor, int T1, int T2){
  if ( DAC == 1 ){
    AnalogOutput1.setValue(Cor);
    delay(T1);
    AnalogOutput1.setValue(Cor);
    delay(T1);
    AnalogOutput1.setValue(2048);
    delay(T2);
  }else{
    AnalogOutput2.setValue(Cor);
    delay(T1);
    AnalogOutput2.setValue(Cor);
    delay(T1);
    AnalogOutput2.setValue(2048);
    delay(T2);
  }
  
}
/*funcion senhla
AnalogOutput2.setValue(3695-ccor);*/

///////////////////////////////////
int ExtraerDados(String mensage){
  
  return envio=1;
}











