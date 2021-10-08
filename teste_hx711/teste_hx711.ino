#include <HX711.h>

// Pinos para a celula de carga ou Load Cell (LC)
#define DOUT  9                                         // D9 Configurado como saída >> Data out LC
#define CLK   10                                        // D10 Configurado como saída >> CLK LC

// Manipulação de pinos para controle do Bluetooth
#define BLE_OUT         DDRA |= (1<<DDB0);              // A0 Configurado como saída
#define BLE_ON          PORTA |= (1<<PORTA0);           // A0 em 1 (manipula a porta entera) >> Liga o Bluetooth On HIGH
#define BLE_OFF         PORTA &= ~(1<<PORTA0);          // A0 em 0 (manipula a porta entera) >> Desliga o Bluetooths Off LOW

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

// Array com comando AT e nome do Dinamo para identificação no App
char ID1[20] = "AT+NAMEE-lastic     ";

bool On = true;


/*********************************************************************************************/
/****************************************    Setup    ****************************************/
/*********************************************************************************************/
void setup() {

  // Configuraçoes iniciais
  BLE_OUT;            // Pin controle do Bt

  delay(250);

  BLE_ON;             // Liga o Bt

  delay(1000);

  Serial.begin(9600); // Habilita comunicação

  delay(1000);

  ble_rename();       // Funçao que Renomea o Bt

  // Teste >> Org: Deixa sempre habilidato
  //Serial.end();       // Deshabilita a comunicação

  delay(1000);

  // Desliga a comunicaçao com o HX711
  scale.power_down();
}



/*********************************************************************************************/
/****************************************    Loop    *****************************************/
/*********************************************************************************************/
void loop() {
  ligado();
}



/*********************************************************************************************/
/**********************************    Função Ligado    **************************************/
/*********************************************************************************************/
void ligado() {

  Serial.begin(9600); // Habilita comunicação

  hx_start(); // Inicializa a LC

  //
  while (On == true) {
    // Calibra o sensor inicialmente com o
    scale.set_scale(calibration_factor);

    LC_read(); // faz a leitura da celula

    Serial.println("Valor medido: ");
    Serial.println(sum);
    delay(500);
  }// fin while On
}


/***********************************************************************************/
/*************    Função para o HX711 ser calibrado inicialmente  ******************/
/***********************************************************************************/
void hx_start() {

  scale.power_up();         // Liga a comunicaçao com o HX711

  delay(500);

  scale.set_scale(calibration_factor); // Configura o factor de escala do CI para medição

  scale.tare(); // Zera a medida
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

  //BLE_OFF;            // Desliga o Bt
  delay(100);
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
}
