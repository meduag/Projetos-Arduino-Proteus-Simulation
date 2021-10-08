#include <SPI.h>

const int ss = 10;
const int led = 14;
int dac = 0; \

void setup() {

  pinMode(ss, OUTPUT);
  pinMode(led, OUTPUT);
  //SPI.setMOSI(7);//estava en 11
  //SPI.setSCK(14);//estava en 13
  SPI.begin();

  digitalWrite(ss, HIGH);
  digitalWrite(led, HIGH);
  delay(200);
  PowerOn_Sequence();
  digitalWrite(led, LOW);
  delay(200);
  digitalWrite(led, HIGH);
  delay(200);
  digitalWrite(led, LOW);
}

void loop() {
  dac = 0;

  // put your main code here, to run repeatedly:
  enviarValorDACs(dac, 65535);
  Act_DACs();
  digitalWrite(led, HIGH);
  delay(400);

  enviarValorDACs(dac, 32768);
  Act_DACs();
  digitalWrite(led, LOW);
  delay(200);

  enviarValorDACs(dac, 0);
  Act_DACs();
  delay(200);

  enviarValorDACs(dac, 32768);
  Act_DACs();
  digitalWrite(led, HIGH);
  delay(200);

  enviarValorDACs(dac, 0);
  Act_DACs();
  delay(200);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////     Para poder hacer el Power-on sequence  ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PowerOn_Sequence() {

  int RW = 0;///para colocar RW = 0 para escritura e 1 para lectura
  int As = 0;
  byte valueH = 0;
  uint16_t valueDB = 0;

  /////// ---- 1 Configuratio register
  valueH = (RW << 7 ) | As;
  valueDB = 32768;
  enviarValorRD(valueH, valueDB);

  /////// ---- 2 Monitor register
  As = 1;
  valueH = (RW << 7 ) | As;
  valueDB = 0;
  enviarValorRD(valueH, valueDB);

  /////// ---- 3 GPIO register grupo A e grupo B
  As = 2;
  valueH = (RW << 7 ) | As;
  valueDB = 57344;
  enviarValorRD(valueH, valueDB);

  /////// ---- 4 Offset DAC register, grupo A e grupo B
  As = 3;
  valueH = (RW << 7 ) | As;
  valueDB = 39322;
  enviarValorRD(valueH, valueDB); //Grupo A
  As = 4;
  valueH = (RW << 7 ) | As;
  enviarValorRD(valueH, valueDB); //Grupo B

  /////// ---- 5 SPI MODE register
  As = 6;
  valueH = (RW << 7 ) | As;
  valueDB = 0;
  enviarValorRD(valueH, valueDB);

  /////// ---- 6 Broadcast register (Update all dacs)
  As = 7;
  valueH = (RW << 7 ) | As;
  valueDB = 0;
  enviarValorRD(valueH, valueDB);
}

void enviarValorRD(byte valueH, uint16_t valueDB) {
  //inicio con las configuraciones
  byte valueM = 0;
  byte valueL = 0;

  valueM = highByte(valueDB);
  valueL = lowByte(valueDB);

  //inicio transmision
  digitalWrite(ss, LOW);
  //enviar Power-on Sequence
  SPI.transfer(valueH);
  SPI.transfer(valueM);
  SPI.transfer(valueL);
  //Termino transmision
  digitalWrite(ss, HIGH);
}



void enviarValorDACs(int A_EndeDac, uint16_t valueDAC) {
  /* Endereço dos Dacs
   * DAC    valor decimal
   * 0          8
   * 1          9
   * 2          10
   * 3          11
   * 4          12
   * 5          13
   * 6          14
   * 7          15
   *
   * ---------------------
   * Os valores de DB15 - DB0 son
   * Valor Decimal    nivel
   * 65356            Alto
   * 32768            Medio
   * 0                Bajo
   */

  int RW = 0;///para colocar RW = 0 para escritura e 1 para lectura
  byte valueH = 0;

  valueH = (RW << 7 ) | A_EndeDac;
  enviarValorRD(valueH, valueDAC);
}

void Act_DACs() {
  int RW = 0;
  int As = 7;
  byte valueH = 0;
  uint16_t valueDB = 0;
  valueH = (RW << 7 ) | As;
  valueDB = 0;
  enviarValorRD(valueH, valueDB);
}

