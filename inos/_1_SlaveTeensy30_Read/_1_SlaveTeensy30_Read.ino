#include <Wire.h>
#include <SPI.h>

#define PinInt1 23

const int ledPin = 0;
//const int T = 5;


//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////              Variables globales            /////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
char mensaje = 'n'; //variable para recibir os caracteres de forma individual
String dados = "<";//Cadena de caracter final
int ss = 10; //Pino para el chip select para el SPI
volatile int updateDados = LOW;//flag Para poder actualizar los datos despues de la prmera vez

int dac = 0;    //Bit de configuracion del Dac para el canal 1 o A
int modop  = 1; //Bit de configuracion para el mopdo de operacion 1
int T1 = 1, T2 = 0, T3 = 0, T4 = 0, mA = 0, Tt = 0, Tr = 0; //Variables globales para onda cuadrada
int cc = 0;//variable para contador de caracteres
char Wf_int = 'T';//Caracter de selecion de onda principal dejando el dac en 0 - (valor 2048)

long Fq = 1000000 / 10;//Variable tipo long para la Frecuencia


//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////            Interrupcion externa - Update dados       //////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
void isrService() {
  //cli();
  digitalWrite(ledPin, LOW);
  Serial.println("Actualizar Dados");
  delay(100);
  updateDados = HIGH;
  digitalWrite(ledPin, HIGH);
  //sei();
   //Wf_int = 'Q';
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////            Configuraciones iniicales         /////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  pinMode(ss, OUTPUT);//Pin chip select configurado como salida

  pinMode(ledPin, OUTPUT);//Pin chip select configurado como salida
  digitalWrite(ledPin, HIGH);//configura el Dac para no recibir dados hasta la actualizacion de los palametros

  pinMode(PinInt1, INPUT); // sets the digital pin as output
  //pinMode(T, INPUT); // sets the digital pin as output
  

 /* if (T == LOW) {
    Wf_int = 'T';
  }
*/
  digitalWrite(ss, HIGH);//configura el Dac para no recibir dados hasta la actualizacion de los palametros

  Wire.begin(1); // Configuracion del device como canal 1 (0x01) para el I2C

  Serial.begin(9600);//Configuracion de velocidad del puerto serial

  SPI.begin();//Configuracion inicio del puerto SPI

  Wire.onReceive(recibidoEvento);//Activa el evento de lectura, run la funcion isrService para cambiar la variable updateDados

  attachInterrupt(PinInt1, isrService, RISING); //Configutacion interrrupcion interna
  Serial.println("\n Iniciando ......");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////            Loop principal         ////////////////////////////////////////////

void loop() {

  if (mensaje != 'n' ) {//solo actualiza si el caracter de inicio cambia
    digitalWrite(ss, HIGH);//configura el Dac para no recibir dados hasta la actualizacion de los palametros

    Serial.println("\n Mansaje final");
    Serial.println(dados);//Imprime los datos recibidos por el control central

    //Actualiza la variable cc contador de caracteres y mostra su valor, es necesaria para otras operaciones
    cc = 0;
    Cont_char();//Contador de caracteres final
    cc = cc + 1; //Contador de caracteres final + 1;
    Serial.println("\n Cantidad de caracteres  ");
    Serial.println(cc);

    //Funcion que separa y actualiza los dados de entrada y actualiza las variables glovales
    separar_datos();

    updateDados = LOW;//Devuelve el valor a 0 para poder crear la señal con el wuile
    dados = "<";//Da un valor inicial a la variable dados
    mensaje = 'n';//Da un valor inicial a la variable mensaje
  }

  //Funcion para poder crear las ondas dependiendo de las variables glovales
  Serial.println("\n basico ------------------------------------------");
  Serial.println("\n Haciendo la primera WaveForm");
  Serial.println("DAC ");
  Serial.println(dac);
  Serial.println("MOP ");
  Serial.println(modop);
  Serial.println("Wave form Inicial");
  Serial.println(Wf_int);

  Sel_wf(Wf_int);

  delay(1000);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////****  Funcion separar y asignar valores de la forma de onda  ****////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void separar_datos() {
  //cont=0;
  char chain_c[cc];   //Declaro una variable tipo char con un maximo de cc (variable dinamica)
  dados.toCharArray(chain_c, cc); //passing the value of the string to the character array

  int cont = 0;//Contador para separar los datos de entrada
  String tmp = "";//Variable temporal para agrupar caracteres

  char d = chain_c[0];//crea una variable para recibir los caracteres y asigna el primero para ver el tipo de onda
  Wf_int = d;//Variable que determina el tipo de Onda a ser creada identificada por el primer caracter (Q - S - E)


  if (d == 'Q') { //Parametros Forma de Ondas y Calculo final de parametros para ella

    //Sepador y actualizador de parametros (vai ate o ultimo caracter)
    for (int i = 1; i <= cc; i++) {
      d = chain_c[i];

      //Separa los datos para Waveform quadrada
      if (cont == 0 && d != ';' ) {
        tmp = tmp + d;
        T1 = tmp.toInt();
      }

      if (cont == 1 && d != ';' ) {
        tmp = tmp + d;
        T2 = tmp.toInt();
      }

      if (cont == 2 && d != ';' ) {
        tmp = tmp + d;
        T3 = tmp.toInt();
      }

      if (cont == 3 && d != ';' ) {
        tmp = tmp + d;
        T4 = tmp.toInt();
      }

      if (cont == 4 && d != ';' ) {
        tmp = tmp + d;
        Fq = tmp.toInt();
        Fq = 1000000 / Fq;

      }

      if (cont == 5 && d != ';' ) {
        tmp = tmp + d;
        mA = tmp.toInt();
      }

      if (d == ';') {//Reinicio la variable temporal de string y aumento el contador para el proximo valor
        tmp = "";
        cont = cont + 1;
      }
    }//fin del for
  }//fin do if

  /*d = chain_c[cc];
    Serial.println("Ultimo caracter ");
    Serial.println(d);*/

  //Imprimo as variables via serial para conferir se os dados chegaram
  Serial.println("Inicio ---- ");
  Serial.println(T1);
  Serial.println(T2);
  Serial.println(T3);
  Serial.println(T4);
  Serial.println(Fq);
  Serial.println(mA);
  Serial.println("Fin ---- ");

}//fin da funcion Separar dados


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////****   Funcion Recibiendo datos y hace una interrupcion interna paran el envio por spi para el dac   ****//////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void recibidoEvento(int recepcion)  // Evento de recepción
{
  dados = "";//Limpio la variable para recibir los nuevos valores en la cadena de caracteres
  //En una posible actualizacion del codigo, aqui se podrian separar los datos inicialmente

  while ( Wire.available())// Leemos hasta que no haya datos y hace una interrupcion del programa principal
  {
    digitalWrite(ss, HIGH);//Desactiva la comunicacion con el dac
    mensaje = Wire.read();//recibe un caracter por vez
    dados = dados + mensaje;//crea la cadena de caracteres con todos los valres
  }
}//fin funcion recibidoEvento


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////****  Funcion Contar la cantidad de carcteres de un array  ****///////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Cont_char()  // Evento de recepción
{
  char ver = dados[cc];//Variable tipo char
  while (ver != '<') // Leemos hasta que no haya datos y hace una interrupcion del programa principal
  {
    cc = cc + 1; //actualizo el valor del contador de caracteres
    ver = dados[cc];//actualizo el valor de la variable ver para ver si llego al final del mensaje recibido
  }
}//fin funcion Cont_char


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////****  Funcion Selecionar WaveForm Q_S_E  ****////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Sel_wf(char WF) {

  ////--- Funcion Selecionar WaveForm Q
  if (WF == 'Q') {
    //dac = 0;//configuracion para el dac canal 1
    //modop = 1; //configuracion modo de operacion 1

    //formas de onda quadrada
    /*

       Forma de Onda 1 con T1 y la Frequencia

       |  T1
       | |---|
       | |   |
       | |   |     --------------------------
       -------------------------------------- 0
              |   |--------------------------
              |   |
              |---|
                T1

       Forma de Onda 2 con T1, T2 y la Frequencia

       |  T1
       | |---|
       | |   |T2
       | |   |---     -----------------------
       -------------------------------------- 0
              ---|   |-----------------------
                 |   |
                 |---|
                  T2

       Forma de Onda 3 con T1, T2 siendo que T3 depende de T4 mas la frequencia

       |  T1              T4
       | |---|           |---|
       | |   |T2      T3 |   |
       | |   |---     ---|   |---------------
       -------------------------------------- 0
              ---|   |---     ---------------
                 |   |
                 |---|
                  T1
    */
    if (T2 == 0 && T3 == 0 && T4 == 0) {
      Tt = (2 * T1);
      Tr = Fq - Tt;

      if (Tr > 0) {
        Onda_Q(1);  //Forma de Onda 1
      } else {
        Serial.println("\n No es posible crear la señal verificar T1");
      }

    } else if ( T3 == 0 && T4 == 0) {
      Tt = (2 * T1) + T2;
      Tr = Fq - Tt;

      if (Tr > 0) {
        Onda_Q(2);  //Forma de Onda 2
      } else {
        Serial.println("\n No es posible crear la señal verificar T1 y T2 ");
      }

    } else {
      Tt = (2 * T1) + T2 + T3 + T4;
      Tr = Fq - Tt;

      if (Tr > 0) {
        Onda_Q(3);  //Forma de Onda 3
      } else {
        Serial.println("\n No es posible crear la señal verificar T1, T2, T3 y T4 ");
      }

    }//fin selecionador de secuencias
  }//fin selicional waveform

  //////Para as outras formas de ondaaaaaaaaaaaaaa

  if (Wf_int == 'T') {
    /*Serial.println("\n basico ------------------------------------------");
      Serial.println("\n Haciendo la primera WaveForm");
      Serial.println("DAC ");
      Serial.println(dac);
      Serial.println("MOP ");
      Serial.println(modop);*/

    /*  int te = 1000;
      for (int i = 0; i <= 10; i++) {
        enviarValor(dac, modop, 2047);
        delay(te);
        enviarValor(dac, modop, 4095);
        delay(te * 2);
        enviarValor(dac, modop, 0);
        delay(te);
      }*/



/*
    int i2 = 0;
    for (int i = 0; i <= 24; i++) {
      i2 = map(i, 0, 100, 0, 2047);
      Serial.println("Miliamperios : ");
      Serial.println(i);
      for (int j = 0; j <= 8000; j++) {
        enviarValor(dac, modop, 2047 + i2);//lado positivo de la señal quadrada
        delayMicroseconds(50);
        enviarValor(dac, modop, 2047 - i2);//lado negativo de la señal quadrada
        delayMicroseconds(50);
        enviarValor(dac, modop, 2047); // valor para ref de la Tierra o 0 Volts
        delayMicroseconds(500);
      }
    }
    */

    enviarValor(dac, modop, 2048);
    delay(1000);
    //enviarValor(dac, modop, 4095);
    //delay(1000);
    //enviarValor(dac, modop, 0);
    //delay(1000);
  }


  ////--- Funcion Selecionar WaveForm Seno
  if (WF == 'S') {
    //Aqui va el codigo para este tipo de Waveform
  }

  ////--- Funcion Selecionar WaveForm Exponencial
  if (WF == 'E') {
    //Aqui va el codigo para este tipo de Waveform
  }

}//fin de la funcion Sel_wf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////****  Funcion para selecionar WaveForm Quadrada con los tiempos  ****//////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Onda_Q(int Wf_Sel) {//Recibe el parametro para crear la forma de onda

  mA = map(mA, 0, 100, 0, 2047);//Mapeo los datos de la variable mA para 2048 siendo el valor +/- de la amplitud
  Serial.println("\n corrente ");
  Serial.println(mA);

  if (Wf_Sel == 1) {//forma de onda 1
    Serial.println("\n Haciendo la primera WaveForm solo con T1 y Frequencia");

    while (updateDados == LOW) {
      enviarValor(dac, modop, 2047 + mA);//lado positivo de la señal quadrada
      delayMicroseconds(T1);
      enviarValor(dac, modop, 2047 - mA);//lado negativo de la señal quadrada
      delayMicroseconds(T1);
      enviarValor(dac, modop, 2047); // valor para ref de la Tierra o 0 Volts
      delayMicroseconds(Tr);
    }
  }

  if (Wf_Sel == 2) {//forma de onda 1
    while (updateDados == LOW) {
      enviarValor(dac, modop, 2048 + mA);
      delayMicroseconds(T1);
      enviarValor(dac, modop, 2048);
      delayMicroseconds(T2);
      enviarValor(dac, modop, 2048 - mA);
      delayMicroseconds(T1);
      enviarValor(dac, modop, 2048);
      delayMicroseconds(Tr);
      Wire.onReceive(recibidoEvento);   // Al activarse el evento de lectura, se ejecuta la funcion recibidoEvento
    }
  }

  if (Wf_Sel == 3) {//forma de onda 1
    while (updateDados == LOW) {
      enviarValor(dac, modop, 2048 + mA);
      delayMicroseconds(T1);
      enviarValor(dac, modop, 2048);
      delayMicroseconds(T2);
      enviarValor(dac, modop, 2048 - mA);
      delayMicroseconds(T1);
      enviarValor(dac, modop, 2048);
      delayMicroseconds(T3);
      enviarValor(dac, modop, 2048 + mA);
      delayMicroseconds(T4);
      enviarValor(dac, modop, 2048);
      delayMicroseconds(Tr);
      Wire.onReceive(recibidoEvento);   // Al activarse el evento de lectura, se ejecuta la funcion recibidoEvento
    }
  }

}//fin funcion Onda quadrada


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////****  Funcion para enviar los datos al DAC por SPI  ****////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void enviarValor(int DacABCD, int MoP, uint16_t value) {
  byte valueToWriteH = 0;//Variable Byte alto
  byte valueToWriteL = 0;//Variable Byte bajo

  valueToWriteH = highByte(value);//Separo el byte alto
  valueToWriteH = 0b00001111 & valueToWriteH;
  valueToWriteH = (DacABCD << 6 ) | (MoP << 4) | valueToWriteH;

  valueToWriteL = lowByte(value);//Separo el byte bajo

  digitalWrite(ss, LOW);//Activo el SPI device para recibor los dados
  SPI.transfer(valueToWriteH);//Envio byte alto
  SPI.transfer(valueToWriteL);//Envio byte bajo
  digitalWrite(ss, HIGH);//Desactivo el SPI device
}

