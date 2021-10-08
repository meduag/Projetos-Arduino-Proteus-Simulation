#include <IRremote.h>
// #include <SoftwareSerial.h>

// For data input and split data
String data_in; // General variable for data in serial communication
const char fin = '>'; // Character separator used for split data '>'
int ok = 0; // para saber si esta todo bien con el mensaje

// Global variables for actions with 0/1 Activation
/************* BOOL *******************/
// Pos Array  Desciption    Activation    Data_in Rx/Tx (>)
// 0          Light 1       on/off        0/1
// 1          Light 2       on/off        0/1
// 2          Light 3       on/off        0/1
// 3          Tv            on/off        0/1
// 4          Air           on/off        0/1

// Cantidad de variables para pinos de Salida
int var_num_out = 3;
int tv_dir = 3;
int are_dir = 4;

// Variables de control
bool b_var[] = {0, 0, 0, 0, 0};

// Cantidad de variables para pinos de entrada
//const uint8_t var_num_in = 3;

// Configuracion de pinos de Salida
const int Lg1 = 10; // Relay Light 1
const int Lg2 = 11; // Relay Light 2
const int Lg3 = 12; // Relay Light 3

// Configuracion de pinos de Entrada
//SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

const int Btn1 = 2; // Boton Light 1 > Principal
const int Btn2 = 3; // Boton Light 2 > Corredor
const int Btn3 = 4; // Boton Light 3 > Banho

const int time_button = 200;

// Array de pinos de salida
int Output_Pins[3] = {Lg1, Lg2, Lg3};

// Array de pinos de Entrada fisicos
int Input_Pins[] = {Btn1, Btn2, Btn3};

int legnth_In = 0, legnth_Out = 0, legnth_bvar = 0;

// Mensaje base
String msn_base = "0>0>0>";


IRsend irsend;  // Un Led IR para controlar en el pin 8 en el atmega8
// const int IRg = 9; // Pin for IR Light ^^^^ ya esta identificado en la biblioteca IRremote

void lee_botones();

void setup() {
  // Inicializo la communicacion
  Serial.begin(115200);
  Serial.setTimeout(100);

  /*Serial1.begin(115200);
  Serial1.setTimeout(100);*/

  // Inicializo los pinos de Salida
  legnth_Out = (sizeof(Output_Pins) / sizeof(Output_Pins[0]));
  for (int i = 0; i < legnth_Out; i++) {
    pinMode(Output_Pins[i], OUTPUT);
    digitalWrite(Output_Pins[i], 0);
  }

  // Inicializo los pinos de Entrada
  legnth_In = (sizeof(Input_Pins) / sizeof(Input_Pins[0]));
  for (int i = 0; i < legnth_In; i++) {
    pinMode(Input_Pins[i], INPUT);
  }

  // Inicializa los valores de b_var[]
  legnth_bvar = (sizeof(b_var) / sizeof(b_var[0]));
  for (int i = 0; i < legnth_bvar; i++) {
    b_var[i] = 0;
  }

  Serial.println("Todo Configurado: OK");
  delay(1000); // Espera 100ms

}// Fin de la Funcion Setup

void loop() {
  // Function para leer datos del ESP8266
  lee_datos_de_ESP();

  // Function para leer botones y actualiza ESP8266
  lee_botones();

  // // Function for updates OUTPUTs
}// End Loop function


/**************************************************************/
/******************** Read input data *************************/
/**************************************************************/
void lee_datos_de_ESP() {
  // Ejemplos de String de entrada
  // 1>1>1>1>1>
  // 0>0>0>1>1>

  // Actualiza todas las Variables
  if (Serial.available() != 0) {
    Serial.println("Separando valores en Tenssy");
    Serial.println(Serial.readString());
   for (int i = 0; i < legnth_bvar; i++) {
      data_in = Serial.readStringUntil(fin);
      Serial.println(data_in);
      if (data_in != "1") {
        if (data_in != "0") {
          Serial.println("Error en el mensaje de entrada");
          ok = 0;
        } else {
          ok = 1; // Mensaje ok para 0
        }
      } else {
        ok = 1; // Mensaje ok para 1
      }

      if (ok == 1) {
        // Solo guarda estados que esten verificadas
        b_var[i] = data_in.toInt();
      }
    }// fin for

    // ---------- Limpia el buffer de entrada  ----------//
    Serial.readString();

    // Inicializo los pinos de salida de las luces
    for (int i = 0; i < legnth_Out; i++) {
      digitalWrite(Output_Pins[i], b_var[i]);
      define_estado(b_var[i], i * 2);
    }

    if (b_var[tv_dir] == 1 ) {
      irsend.sendSony(0xA90, 12);
    } else {
      irsend.sendSony(0xA90, 12);
    }

    /*if (b_var[aire_dir] == 1 ) { // Cambiar HEX para el aire
       irsend.sendNEC(0xB24D7B‬, 32);
      } else {
       irsend.sendNEC(0xB24D7B‬, 32);
      }*/

  }// End if serial available
}// End read_data function

void lee_botones() {
  // leo los estados de los botones
  if (digitalRead(Btn1) == 0) {
    delay(time_button);
    //Serial.println("Btn 1 Cambio");
    b_var[0] = !b_var[0];
    digitalWrite(Lg1, b_var[0]);
    define_estado(b_var[0], 0);
    Serial.println(msn_base);
    //Serial1.println(msn_base);
  }

  if (digitalRead(Btn2) == 0) {
    delay(time_button);
    //Serial.println("Btn 2 Cambio");
    b_var[1] = !b_var[1];
    digitalWrite(Lg2, b_var[1]);
    define_estado(b_var[1], 2);
    Serial.println(msn_base);
    //Serial1.println(msn_base);
  }

  if (digitalRead(Btn3) == 0) {
    delay(time_button);
    //Serial.println("Btn 3 Cambio");
    b_var[2] = !b_var[2];
    digitalWrite(Lg3, b_var[2]);
    define_estado(b_var[2], 4);
    Serial.println(msn_base);
    //Serial1.println(msn_base);
  }
}

void define_estado(bool estado, int dir) {
  if (estado == true) {
    char boton = '1';
    msn_base.setCharAt(dir, boton);
  } else if (estado == false) {
    char boton = '0';
    msn_base.setCharAt(dir, boton);
  }
}
