#include <HX711.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <EEPROM.h>

//MACROS
#define DOUT  9
#define CLK   10

// Manipulação de pinos para controle do Bluetooth
#define BLE_OUT         DDRA |= (1<<DDB0);              // A0 como saída
#define BLE_ON          PORTA |= (1<<PORTA0);           // A0 em 1 (manipula a porta entera)
#define BLE_OFF         PORTA &= ~(1<<PORTA0);          // A0 em 0 (manipula a porta entera)

// Manipulação de pinos para controle/monitoramento Bateria
#define BAT_LED_OUT     DDRA |= (1<<DDA6);              // A6 como saída
#define BAT_LED_IN      DDRA &= ~(1<<DDA6);             // A6 como entrada
#define BAT_LED_ON      PORTA |= (1<<PORTA6);           // A6 em 1 (manipula a porta entera) led bateria On
#define BAT_LED_OFF     PORTA &= ~(1<<PORTA6);          // A6 em 0 (manipula a porta entera) led bateria Off
#define BAT_LED_TOGGLE  PINA |= (1<<PINA6);             // A6 em 0 (manipula o pin individual) led bateria Pisca

// Pino para leer a tensao de entrada (5V) quando esta carregando
#define TP_read_IN      DDRA &= ~(1<<DDA3);

// Pin para leer boton capacitivo
#define BTN_IN          DDRB &= ~(1<<DDB2);

//VARIÁVEIS DA CELULA DE CARGA
//long int calibration_factor = -88000; //50k
long int calibration_factor = 24998; //200kg TS >>>> media do valor
//long int calibration_factor = -8900; //500kg TSM

//[segundos] tempo antes do dispositivo auto-desligar
const uint16_t tempo_ocioso = 300;

// Pino analogico para leitura da tensao da bateria, levar em consderação
// que esa sendo usada a tensao de referencia interna.
const uint8_t BAT_read = A7;

// Variavel para concatenar dados da celula de carga e enviar via serial
long int  sum;


// Identificação dos pinos do Objeto da celula
HX711 scale(DOUT, CLK);

// Numero de caracteres usados para receber a informação da comunicação e executar tarefas
int num_char_in = 2;

// coracteres de entrada usados para a comunicação
char c[num_char_in];

/*********************/
uint8_t rate = 8; // Número de leituras/amostras a ser tomadas
float x[8]; // matriz com 8 valores float para lritura da LC
uint16_t b = 0; // ?????
uint16_t i = 0; // ?????

// Variavel de controle de fluxo do programa liga/desliga
boolean On = true;

// ??????
int inicio;

ISR (PCINT1_vect) {} // parece que nao tem nada com esta funcion ou tem alguma func=ção especial?



/*********************************************************************************************/
/****************************************    Setup    ****************************************/
/*********************************************************************************************/
void setup() {

  // Configuraçoes iniciais
  BLE_OUT;            // Pin controle do Bt
  BLE_OFF;            // Desliga o Bt

  delay(250);

  BAT_LED_OUT;        // Pin controle Led da bateria
  BAT_LED_ON;         // Liga o Led
  TP_read_IN;         // Pin de leitura da tensao de carga de entrada
  BTN_IN;             // Pin de leitura do Botao capacitivo
  BLE_ON;             // Liga o Bt

  delay(1000);

  Serial.begin(9600); // Habilita comunicação

  delay(1000);

  ble_rename();       // Funçao que Renomea o Bt

  Serial.end();       // Deshabilita a comunicação

  delay(1000);

  BLE_OFF;            // Desliga o Bt

  // Desliga a comunicaçao com o HX711
  scale.power_down();

  // Referecia de tensao analogica interna para comparação nas entradas analogicas
  analogReference(INTERNAL1V1);

}



/*********************************************************************************************/
/****************************************    Loop    *****************************************/
/*********************************************************************************************/
void loop() {
  if (On == true)
    ligado();

  if (On == false)
    desligado();

}

void ligado() {
  Serial.begin(9600);
  hx_start();

  //
  while (on == true) {
    scale.set_scale(calibration_factor);
    if (Serial.available()) {
      lee_entrada(); // fazer leitura e comprobar que função vai ser executada
      if (c[0] == 'E') {
        new_receiver(); //
      } else {
        new_LC_calibration(); // faz a calibração da celula
      }
    }

    LC_read(); // faz a leitura da celula

    // manda mas de 2 bytes
    if (sum > 255) {
      int num_escala;
      num_escala = sum / 255;
      if (num_escala >= 1)
        Serial.write(num_escala);  // num escala >>>
    }

  }
}


/***********************************************************************************/
/************************    HX711 leitura inicial   *******************************/
/***********************************************************************************/
void hx_start() {

  // read_EEPROM();

  scale.power_up(); // Liga a comunicaçao com o HX711
  delay(500);

  scale.set_scale(calibration_factor); // Configura o factor de escala

  scale.tare(); // Zera a medida
}


/***********************************************************************************/
/************************    Lee os dados de entrada   *****************************/
/***********************************************************************************/
void lee_entrada() {
  for (int i = 0; i < num_char_in; i++) {
    c[i] = Serial.read(); // almacena em C
  }
}


/***********************************************************************************/
/******************    RECEIVER - Funçoes comuninaçao 1   **************************/
/***********************************************************************************/
void new_receiver() {
  scale.set_scale(); // Configura a escala

  switch (c[0]) {
    case '0':
      // Envia o valor do factor de calibração >>>>>>>>>>>>>>>>>>> Função repetida >>>>>>>>>>>>>>>>>>>
      Serial.println(calibration_factor);
      break;
    case '1':
      // Envia o valor do pesso dependendo da escala medida
      long int calibrationConst = scale.get_units(10) / 48.6;
      Serial.println(calibrationConst);
      delay(5000);
      break;
    case '2':
      // Chama a função da bateria
      battery();
      break;
    case '3':
      // Lee a porta, Ajusta e imprime o valor do rate
      rate = Serial.read() - 48; // converte para int
      Serial.println(rate);
      break;
    case '4':
      // Chama função de calibração
      new_LC_calibration();
      break;
    case '7':
      // Envia o valor do factor de calibração >>>>>>>>>>>>>>>>>>> Função repetida >>>>>>>>>>>>>>>>>>>
      Serial.println(calibration_factor);
      break;
    case '8':
      // Desliga o dinamo
      on = false;
      break;
    case '9':
      // Envia o valor do factor de calibração >>>>>>>>>>>>>>>>>>> Função repetida >>>>>>>>>>>>>>>>>>>
      Serial.println(calibration_factor);
      break;
  }
  // Espera 2s
  delay(2000);
}


/***********************************************************************************/
/**************    new_LC_calibration - Funçoes comuninaçao 2   ********************/
/***********************************************************************************/
void new_LC_calibration()() {

  int calibration_division = 0;
  int calibration_rest = 0;
  int eeAddress = 0;
  int sinal = 0;
  int zero = 0;

  scale.set_scale(); // Configura a escala

  switch (c[0]) {
    case '1':
      // Envia o valor do pesso dependendo da escala medida
      scale.set_scale();
      long int calibrationConst = scale.get_units(10) / 48.8;
      delay(500);
      calibration_factor = calibrationConst;
      Serial.print(calibration_factor);
      break;

    case '2':
      // Ajusta o valor do factor de calibração fazendo incrementos de 50 unidades
      calibration_factor = calibration_factor + 50;
      delay(500);
      Serial.print(calibration_factor);
      delay(1500);
      break;

    case '3':
      // Ajusta o valor do factor de calibração fazendo decrementos de 50 unidades
      calibration_factor = calibration_factor - 50;
      delay(500);
      Serial.print(calibration_factor);
      delay(1500);
      break;

    case '4':
      // Salva o valor do factor de calibração na EEPROM
      write_EEPROM();
      break;

    case '5':
      // Lee a EEPROM e envia o valor
      read_EEPROM();
      delay(500);
      Serial.print(calibration_factor);
      delay(1500);
      break;

    case '6':
      // Escreve na EEPROM tres valores de Zero >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Why?????????????????????????????????
      eeAddress = 0;
      zero = 0;
      EEPROM.write(eeAddress, zero);
      eeAddress += 1;
      EEPROM.write(eeAddress, zero);
      eeAddress += 1;
      EEPROM.write(eeAddress, zero);
      break;

    case '7':
      // Envia o valor do factor de calibração
      Serial.println(calibration_factor);
      delay(3000);
      break;

    case '8':
      // Desliga o Dinamo
      on = false;
      break;

    case '9':
      // Chama a função da bateria
      battery();
      break;
  }
}


/***********************************************************************************/
/************************    Lee os dados de entrada   *****************************/
/***********************************************************************************/
