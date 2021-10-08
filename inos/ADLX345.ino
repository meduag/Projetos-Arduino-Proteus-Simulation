#include <Wire.h>

//Defino registros
#define accel_module (0x53) // Adress del Acelerometro - Para comunicar con el acelerometro 
#define POWER_CTL (0x2D) // registro de lectura y escritura
#define DATA_FORMAT (0x31) //Registro de seleccion de sensibilidad 0x00 para 2g - ver tabla 21 Range Setting para otros valores

//defino los pinos de control del estimulador
#define SYNC_PIN  2 // //pino que inicia la sincronizacion de datos
#define STIM_PIN_IN 3 // pino de iniocio y final del pulso
#define CTRL_BLINK 13 // pino de iniocio y final del pulso

//Inicializo Variables
byte values[6]; // para guardar los valore LSB HSB de los ejes
char output[512]; // para concatenar el mensaje de salida

//registros de las variables
int xyzregister = 0x32; // registro de los ejes para medida
int x, y, z; //valores de los ejes en entero

int ctrl = 0; //Variable de control
int j = 0; //para el while de separacion de bytes XYZ
int stim_sync = 0; //para gravar el tiempo del pulso

void setup() {
  Wire.begin();//Inicio comunicacion I2C
  Serial.begin(115200);//Inicio comunicacion serial

  //configuraciones del acelerometro
  Wire.beginTransmission(accel_module);//Frecuencia de lectura en sleep mode
  Wire.write(POWER_CTL);
  Wire.write(0);// 8Hz
  Wire.endTransmission();

  Wire.beginTransmission(accel_module);//Auto Sleep Mode - Activado
  Wire.write(POWER_CTL);
  Wire.write(16);
  Wire.endTransmission();

  //Configurando el puerto para medir
  Wire.beginTransmission(accel_module);//- Habilito transmision
  Wire.write(POWER_CTL);
  Wire.write(8);// 0B1000 - activando medida (D3 - Measure)
  Wire.endTransmission();// - Cierro transmision

  //Configuro la sensibilidad
  Wire.beginTransmission(accel_module);//- Habilito transmision
  Wire.write(DATA_FORMAT);
  Wire.write(0);// seleciono 2g
  Wire.endTransmission();//- Cierro transmision

  //Configuracion de pinos de entrada
  pinMode(SYNC_PIN, INPUT);//confg pin digital, como entrada
  pinMode(STIM_PIN_IN, INPUT);//confg pin analogico, como entrada
  pinMode(CTRL_BLINK, OUTPUT);//confg pin analogico, como entrada

  digitalWrite(CTRL_BLINK, HIGH); //habilita el pino  alto, esperando conexion

}//Fin setup

void loop() {

  //esperando iniciacion do Estimulador
  if (ctrl == 0) {
    digitalWrite(CTRL_BLINK, HIGH); //habilita el pino  alto, esperando conexion
    digitalWrite(STIM_PIN_IN, LOW); //Sin pulsos
    stim_sync = 0;
  }

  //envia dados para gravacion si el SYNC_PIN se activa
  while (digitalRead(SYNC_PIN)) {

    //int t0 = millis();
    //Serial.println(t0);

    stim_sync = digitalRead(STIM_PIN_IN);

    if (ctrl == 0) {
      ctrl = 1;
      digitalWrite(CTRL_BLINK, LOW);
    }

    //while (millis() - t0 < 1000) {

      //Comienza la adquisicion de datos
      Wire.beginTransmission(accel_module);
      Wire.write(xyzregister);
      Wire.endTransmission();

      //requisita las medidas
      Wire.beginTransmission(accel_module);
      Wire.requestFrom(accel_module, 6);

      //separa en bytes recibidos de los ejes XYZ
      j = 0;
      while (Wire.available()) {
        values[j] = Wire.read();
        j++;
      }
      Wire.endTransmission();

      x = (((int)values[1]) << 8) | (int)values[0];
      y = (((int)values[3]) << 8) | (int)values[2];
      z = (((int)values[5]) << 8) | (int)values[4];

      
      //stim_sync = stim_sync + 1;

      sprintf(output, "%d ; %d  ; %d  ; %d", stim_sync, x, y, z);
      Serial.println(output);//imprimo la cadena completa


    //}//Fin while time
  }//fin while boton

  if (ctrl == 1) {
    digitalWrite(CTRL_BLINK, LOW); //habilita el pino  alto, esperando conexion
    ctrl = 0;
    delay(1000);
  }

}
