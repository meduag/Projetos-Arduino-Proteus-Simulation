#include <avr/sleep.h>
#include <avr/power.h>
#include <HX711.h>
#include <EEPROM.h>

// Pinos para a celula de carga ou Load Cell (LC)
#define DOUT  9                                         // D9 Configurado como saída >> Data out LC
#define CLK   10                                        // D10 Configurado como saída >> CLK LC

// Manipulação de pinos para controle do Bluetooth
#define BLE_OUT         DDRA |= (1<<DDB0);              // A0 Configurado como saída
#define BLE_ON          PORTA |= (1<<PORTA0);           // A0 em 1 (manipula a porta entera) >> Liga o Bluetooth On HIGH
#define BLE_OFF         PORTA &= ~(1<<PORTA0);          // A0 em 0 (manipula a porta entera) >> Desliga o Bluetooths Off LOW

// Manipulação de pinos para controle/monitoramento Bateria
#define BAT_LED_OUT     DDRA |= (1<<DDA6);              // A6 Configurado como saída >> Controla o led de indicação da bateria OUTPUT
#define BAT_LED_IN      DDRA &= ~(1<<DDA6);             // A6 Configurado como entrada >> Controla o led de indicação da bateria INPUT digital
const uint8_t CHG_read = A6;
// Para o novo CI de carga MCP73833 o STATE1 liga en LOW - Teste
#define BAT_LED_OFF     PORTA |= (1<<PORTA6);           // A6 em 1 (manipula a porta entera o pin pontual) led bateria On HIGH
#define BAT_LED_ON      PORTA &= ~(1<<PORTA6);          // A6 em 0 (manipula a porta entera o pin pontual) led bateria Off LOW
/* Original
  #define BAT_LED_ON      PORTA |= (1<<PORTA6);           // A6 em 1 (manipula a porta entera o pin pontual) led bateria On HIGH
  #define BAT_LED_OFF     PORTA &= ~(1<<PORTA6);          // A6 em 0 (manipula a porta entera o pin pontual) led bateria Off LOW
*/
#define BAT_LED_TOGGLE  PINA |= (1<<PINA6);             // A6 em 0 (manipula o pin individual) led bateria alternancia

// Pino para leer a tensao de entrada (5V) quando esta carregando
#define TP_read_IN    DDRB &= ~(1<<DDB2);              // B2 Configurado como entrada >> INPUT botao Touch

// Pin para leer boton capacitivo
#define IN_STATE      DDRA &= ~(1<<DDA3);              // A3 Configurado como entrada >> INPUT carregador conectado - antigo BTN_IN 

//VARIÁVEIS DA CELULA DE CARGA
//long int calibration_factor = -88000; //50k
long int calibration_factor = 24998; //200kg TS >>>> media do valor
//long int calibration_factor = -8900; //500kg TSM

//[segundos] tempo antes do dispositivo auto-desligar
const uint16_t tempo_ocioso = 300;

// Pino analogico para leitura da tensao da bateria, levar em consderação
// que esa sendo usada a tensao de referencia interna.
const uint8_t BAT_read = A7;                            // A7 Configurado como entrada analogica >> Controla o led de indicação da bateria INPUT

// Variavel para concatenar dados da celula de carga e enviar via serial
long int  sum;

//Teste
// para identificar o estado da bateria e a carga da mesma
// 0 = Bateria descarregando
// 1 = Batria quase descarregada (Led pisca constatemente em um tempo determinado)
// 2 = Bateria descarregada
// 3 = bateria full carregada
int estado_bateria = 0;
int estado_bateria_string = 0;


// Identificação dos pinos e criação do Objeto da celula de carga para o CI HX711
HX711 scale(DOUT, CLK);

//Teste
// Numero de caracteres usados para receber a informação da comunicação e executar tarefas
int num_char_in = 2;

//Teste
// coracteres de entrada usados para a comunicação
char c[2];

/*********************/
uint8_t rate = 8; // Número de leituras/amostras a ser tomadas da LC
float x[8]; // matriz com 8 valores float para leitura da LC
uint16_t b = 0; // Variavel global usada no delay de desligamento do Aparelho
uint16_t i = 0; // ?????

// Variavel de controle de fluxo do programa liga/desliga
boolean On = true;

// ??????
int inicio;

ISR (PCINT1_vect) {} // parece que nao tem nada com esta funcion ou tem alguma func=ção especial?

// Array com comando AT e nome do Dinamo para identificação no App
char ID1[20] = "AT+NAMEE-lastic     ";

// Teste
//comnicacion
int uno = 1;

/*********************************************************************************************/
/****************************************    Setup    ****************************************/
/*********************************************************************************************/
void setup() {

  // Configuraçoes iniciais
  BLE_OUT;            // Pin controle do Bt

  delay(250);

  BAT_LED_OUT;        // Habilita o pin controle Led da bateria
  BAT_LED_ON;         // Liga o Led Bateria
  delay(100);
  BAT_LED_OFF;
  TP_read_IN;         // Habilita o pin de leitura do Botao capacitivo
  IN_STATE;           // Habilita o pin de leitura da tensao de carga de entrada
  BLE_ON;             // Liga o Bt

  delay(1000);

  Serial.begin(9600); // Habilita comunicação

  delay(1000);

  ble_rename();       // Funçao que Renomea o Bt

  // Teste >> Org: Deixa sempre habilidato
  Serial.end();       // Deshabilita a comunicação

  delay(1000);

  // Desliga a comunicaçao com o HX711
  scale.power_down();

  // Referecia de tensao analogica interna para comparação nas entradas analogicas
  analogReference(INTERNAL1V1);

  BLE_ON;

}



/*********************************************************************************************/
/****************************************    Loop    *****************************************/
/*********************************************************************************************/
void loop() {

  if (On == true) {
    ligado();
  }

  if (On == false) {
    desligado();
  }

}


/*********************************************************************************************/
/**********************************    Função Ligado    **************************************/
/*********************************************************************************************/
void ligado() {

  Serial.begin(9600); // Habilita comunicação
  BLE_ON; // liga BTH
  BAT_LED_ON; // Liga led Bateria
  hx_start(); // Inicializa a LC

  while (On == true) {
    // Calibra o sensor inicialmente com o
    scale.set_scale(calibration_factor);

    if (Serial.available() > 0) {
      lee_entrada(); // fazer leitura e comprobar que função vai ser executada
      if (c[0] == 'E') {
        //Serial.println("funçao antiga");
        new_receiver(); // Versao antiga
      } else {
        new_LC_calibration(); // Versao nova - faz a calibração da celula
        //Serial.println("funçao nova");
      }
    }

    LC_read();

    if (sum > 255) {
      int num_escala;
      num_escala = sum / 255;
      if (num_escala >= 1) {
        Serial.write(num_escala);
      }
    }

    Serial.write(sum); // original

    battery2();

    if (PINB & (1 << PINB2)) { //digitalRead(BTN) == HIGH)
      btn();
    }

    if (PINA & (1 << PINA3)) { //digitalRead(TP_read) == HIGH)
      On = false;
    }

  }// fin while On

  delay(200);
  scale.power_down();
  BLE_OFF;
  BAT_LED_ON;
  Serial.end();
}


/***********************************************************************************/
/*************    Função para o HX711 ser calibrado inicialmente  ******************/
/***********************************************************************************/
void hx_start() {
  read_EEPROM();            // Atualização do factor de calibração lendo diretamente da EEPROM

  scale.power_up();         // Liga a comunicaçao com o HX711

  delay(500);

  scale.set_scale(calibration_factor); // Configura o factor de escala do CI para medição

  scale.tare(); // Zera a medida
}


/***********************************************************************************/
/**************      Função para Ler os dados de entrada   *************************/
/***********************************************************************************/
void lee_entrada() {
  for (int i = 0; i <= num_char_in; i++) {
    c[i] = Serial.read(); // almacena em C
    //Serial.println(c[i], );
  }
  Serial.read();// Limpa o Buffer
}


/***********************************************************************************/
/******************    RECEIVER - Funçoes comuninaçao 1   **************************/
/***********************************************************************************/
void new_receiver() {
  scale.set_scale(); // Configura a escala (que mais faz?)

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
      On = false;
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
void new_LC_calibration() {

  // int calibration_division = 0;
  // int calibration_rest = 0;
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
      // Zera a EEPROM
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
      On = false;
      break;

    case '9':
      // Chama a função da bateria e imprime o valor da tensao
      battery();
      break;
  }
}


/***********************************************************************************/
/***************    Função para renomear o modulo BT   *****************************/
/***********************************************************************************/
void ble_rename() {
  BLE_OFF; // Deliga e liga o Bluetooth
  delay(100);
  BLE_ON;
  delay(100);

  Serial.write("AT+RESET"); // Reiniciar Módulo Bt
  Serial.println();
  delay(500);


  Serial.write("AT+SAVE0"); // Configura o módulo para que nao salve o último endereço conectado
  Serial.println();
  delay(500);


  Serial.write(ID1); // Renomea o Módulo Bt para E-Lastic
  Serial.println();
  delay(500);

  BLE_OFF;            // Desliga o Bt
  delay(100);
}


/***********************************************************************************/
/************************   Função para Ler a EEPROM   *****************************/
/***********************************************************************************/
void read_EEPROM() {

  int calibration_division = 0;
  int calibration_rest = 0;
  int eeAddress = 0;
  int sinal = 0;

  sinal = EEPROM.read(eeAddress); // lê a primeira posisçao da EEPROM para saber a Polaridade

  //Para poder colocar o valor do factor de calibração positivo
  if (sinal == 1) {
    sinal = -1;
  } else {
    sinal = 1;
  }

  eeAddress += 1;
  calibration_division = EEPROM.read(eeAddress);            // lê a Segunda posição da EEPROM
  eeAddress += 1;
  calibration_rest = EEPROM.read(eeAddress);                // lê a Terceira posição da EEPROM
  calibration_factor = ((calibration_division * 100) + calibration_rest);  // Concatena o valor final da EEPROM

  if (calibration_factor < 12000) { //////// WHY???????????????/
    calibration_factor = calibration_factor * 8;
  }

  calibration_factor = sinal * calibration_factor; // Valor de calibraçao positivo
}


/***********************************************************************************/
/**************************   Função para Ler a LC   *******************************/
/***********************************************************************************/
void LC_read() {

  for (uint8_t g = 0; g < 7; g++) {
    x[g] = x[g + 1];
  }

  x[7] = 0;

  for (uint8_t g = 0; g < rate; g++) {
    x[7] = x[7] + scale.get_units();
  }

  x[7] = x[7] / rate;

  sum = ((x[0] + x[1] + x[2] + x[3] + x[4] + x[5] + x[6] + x[7]) * 10) / 16;

  sum = sum + 5; // manter a medida positiva

  if (sum < 0) { // Valor positivo
    sum = 0;
  }

  ocioso(); // Manda a ficar ocioso
}


/***********************************************************************************/
/************************   Função para Escrever a LC   ****************************/
/***********************************************************************************/
void write_EEPROM() {

  int calibration_division = 0;
  int calibration_rest = 0;
  int eeAddress = 0;
  int sinal = 0;

  if (calibration_factor > 0)  {
    sinal = 0;
    EEPROM.write(eeAddress, sinal);
  } else {
    sinal = 1;
    EEPROM.write(eeAddress, sinal);
    calibration_factor = calibration_factor * (-1);
  }

  if (calibration_factor > 25000) {
    calibration_factor = calibration_factor / 8;
  }

  // Separa em dois bytes
  eeAddress += 1;
  calibration_division = calibration_factor / 100;
  calibration_rest = calibration_factor % 100;
  EEPROM.write(eeAddress, calibration_division);

  eeAddress += 1;
  EEPROM.write(eeAddress, calibration_rest);
}


/***********************************************************************************/
/*********************   Função para conferir o nao uso   **************************/
/***********************************************************************************/
void ocioso() {
  if (sum < 10) {
    i++;
  } else {
    i = 0;
  }

  if (i >= (tempo_ocioso * 80 / rate)) {
    On = false;
    i = 0;
  }
}


/***********************************************************************************/
/**************************   Função para Bateria 1   ******************************/
/***********************************************************************************/
void battery() {
  Serial.println(analogRead(BAT_read));
  delay(500);
}


/***********************************************************************************/
/**************************   Função para Bateria 1   ******************************/
/***********************************************************************************/
void battery2() { // Pisco o led e controlo fluxo do programa liga/desliga
  BAT_LED_OUT;        // Habilita o pin controle Led da bateria

  int lee = analogRead(BAT_read);

  // Bateria descarregada
  if (lee < 820) {  //801 = 3.697V na bateria totalmente descarregada
    On = false;
    BAT_LED_OFF;
    // Pisca LED BAT con 200ms (pisca rapido)
    for (int r = 0; r < 6; r++) {
      delay(200);
      BAT_LED_TOGGLE;
    }
  } else { // Bateria carregada mas com diferentes opçoes
    // Bateria está descargada e pisca led devagar
    if (lee > 820 && lee < 843 ) {  //820 = 3.785V na bateria
      if ((b % (80 / rate) ) == 0) { // original ""(((b) % (80 / rate)) == 0)""
        BAT_LED_TOGGLE;
      }
      b++;
    } else {
      // Bateria está cargada - !!!!!!  >>>>>>>> descubrir si es 910 mesmo
      if (lee > 843 && lee < 910) {  //820 = 3.785V na bateria
        BAT_LED_ON;
        On = true;
      }

      // Bateria está cargada - !!!!!!  >>>>>>>> descubrir si es 910 mesmo
      if (lee > 926) {
        On = true;
        BAT_LED_OFF; // Tem que ser em 1 logico para desligar >>> conferir declaração de pino
        delay(200);
      }
    } // fin else bateria full/quase full
  }// fin else opçoes full/descarregamento bateria
}


/***********************************************************************************/
/**************************   Função desliga o Dinamo  *****************************/
/***********************************************************************************/
void desligado() {
  while (On == false) {
    BAT_LED_IN;   // Coloca el pino de entrada da bateria como leitura
    /* if (analogRead(CHG_read) < 700){
       colocar aqui lo que se va ha hacer
      }*/
    //aqui se puede leer el pino y decir que ya esta cargado
    sleep_elastic();
    BAT_LED_OUT;
    BAT_LED_ON;
    if (PINB & (1 << PINB2)) { //digitalRead(BTN) == HIGH)
      btn();
    }
    battery2();
  }
}


/***********************************************************************************/
/**************************   Função desliga o Dinamo  *****************************/
/***********************************************************************************/
void sleep_elastic() {
  GIMSK   |= bit (PCIE1);
  GIFR    |= bit (PCIF1);                   // limpe quaisquer interrupção pendente
  PCMSK1  |= bit (PCINT10);                 // SE MUDAR O PINO DO BOTAO, TEM QUE MUDAR O PCINT TAMBEM
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  ADCSRA = 0;                               // coloca os ADCs a 0
  power_all_disable();                      // deshabilita todos os ADCs, Timer 0 and 1, e a interface serial
  sleep_enable();
  sleep_cpu();
  sleep_disable();
  power_all_enable();
  ADCSRA = 135;
}



/***********************************************************************************/
/*****************************   Função do Botao   *********************************/
/***********************************************************************************/
void btn() {
  uint64_t timer = millis();
  while ((millis() - timer < 2000) && (PINB & (1 << PINB2)));
  if (PINB & (1 << PINB2)) { // Liga o aparelho
    On = !On;
  }
}
