#include <SPI.h>

// configuracion datos del dac
#define DAC_OP_MODE 1
#define DAC_SS_PIN 10

// definicion pinos del hardware
#define RELAY_CH_1 9 // rele del canal 1
#define RELAY_CH_2 8 // rele del canal 2
#define RELAY_CH_3 7 // rele del canal 3
#define RELAY_CH_4 6 // rele del canal 4
#define STOP_BUTTON 5 // Boton de emergencia

// Deficion de varaibles/numeros usados en las funciones de desempeño y control
#define V_MAX 4095 // Valor maximo heredado del DAC 12bits a decimal
#define V_MIN 0 // Valor minimo Heredado del DAC
#define V_MED 2047 // Valor medio correspondiente al punto 0


/*************************************************************************************/
/*********************** Variables Globales  *****************************************/
/*************************************************************************************/
String DataIn = ""; // Variable que captura los datos de entrada por el bluetooth
String Val = ""; // Variable que agrupa los caracteres
// Bufer para datos de conversion del String de entrada de los parametros aunque solo se necesitan 38
int TamBuf = 50;
// Array de Strings que seran guardados por ahora son 13 datos
const int Elementos = 13;
String PE[13];
// Parametrso de Estimulacion en String
String Time_therapy = "", intensity = "", freq = "", PWidth = "", On_time = "",
       Off_time = "", ramp = "", Wf = "";
//PE en unidades numericas - valores de tiempo en uS, Hz y mA
unsigned long v_Time_therapy = 0, v_intensity = 0, v_PWidth = 0, v_On_time = 0,
              v_Off_time = 0, v_ramp = 0;
double  v_freq = 0;



// Para la actiacion de los canalaes de estimulacion True= Salida canal On, false = resistencia de precarga
boolean Ch1 = false, Ch2 = false, Ch3 = false, Ch4 = false;
int vCh1 = 0, vCh2 = 0, vCh3 = 0, vCh4 = 0;
// Para saber si es Bipolar = True o Monopolar = false
boolean Pl = true;

///Caracter de separacion de datos, de inicio y fin de los mensajes
const char Ini = '{', csp = ';', Fin = '}';
const char Ini2 = '[', Fin2 = ']';

/*************************************************************************************/
/***************** Variables para funciones de desempeño  ****************************/
/*************************************************************************************/
unsigned long micros_act_th = 0, micros_temp_th = 0; // us
unsigned long micros_act_t_on = 0, micros_temp_t_on = 0; // us
unsigned long micros_act_t_of = 0, micros_temp_t_of = 0; // us
unsigned long micros_act_pw = 0, micros_temp_pw = 0; // us
unsigned long micros_act_fq = 0, micros_temp_fq = 0; // us
const unsigned long time_uS = 100000;//midificar para testes valor original:  1000000
const unsigned long time_mS = 1000;
const int seg_min = 60;
unsigned long Periodo = 0;
//Variable auxiliar para calcular si existe una PW en Off
unsigned long v_PWidth_off = 0, v_intensity_DAC = 0; //el valor puede tener valores negativos pero eso se restringe en el App - Android

//Variables para identificar si fueron recibidos nuevos datos, actualizaciones o boton de emergencia
boolean CanalesActivos = false, SalidaTh_BtnEmergencia = false;
byte opStim = 0b00000000; // valor para selecionar que canal debe ser estimulado


/*************************************************************************************/
/************************* Declaracion de funciones **********************************/
/*************************************************************************************/
void InhabilitarCanales();
void Inicializar(int loop);
void SepararParametros(String Parametros, int TamBuf);
void ConvertirUnidadesNumericas();
void ImprimePE(boolean An_Ar);
void Estimulacion_Start();
void SwitchSeparaParametrosIndex(int i);
void VerificarCanalesActivos();
void BotonEmergencia();



/***************************************************************************************/
/******************************* Configuraciones Iniciales *****************************/
void setup() {
  // Inicia a serial
  Serial1.begin(9600);
  Serial.begin(9600);
  // DAC
  pinMode(DAC_SS_PIN, OUTPUT);
  digitalWrite(DAC_SS_PIN, HIGH);
  SPI.begin();
  // Stop button ---- Mejorar para usar una interrupcion externa
  pinMode(STOP_BUTTON, INPUT);
  // Relays
  pinMode(RELAY_CH_1, OUTPUT);
  pinMode(RELAY_CH_2, OUTPUT);
  pinMode(RELAY_CH_3, OUTPUT);
  pinMode(RELAY_CH_4, OUTPUT);
  // /Cuando se coloca en HIGH habiilita la salida del canal
  // /Cuando se coloca en LOW habiilita la precarga
  digitalWrite(RELAY_CH_1, LOW);
  digitalWrite(RELAY_CH_2, LOW);
  digitalWrite(RELAY_CH_3, LOW);
  digitalWrite(RELAY_CH_4, LOW);
  // Funcion para cerar los canales - Cero todos los canales antes de cualquier comunicacion
  InhabilitarCanales();
}

void loop() {
  // Inicializa el progrograma, Lo primero que hace es esperar la entrada de datos
  Inicializar(1);
  //Reinicio la variable de nuevos datos para usar en Estimulacion_Start
  CanalesActivos = false;

  //Si quiero imprimir tanto los valores separados (true) y los valores numericos usados en el firmware (false)
  ImprimePE(true);
  ImprimePE(false);

  //Crea el estimulo y activa las salidas dependiendo de los valores de entrada
  Estimulacion_Start();

  while (SalidaTh_BtnEmergencia == true) {
    BotonEmergencia();
    Serial.println("Boton de Emergencia activado");
  }

  // Aqui se toman las acciones

}

/********************************************************************************/
/************************** Funciones Auxiliares  *******************************/
/********************************************************************************/
// Funcion para recibbir y separar los datos
// No sale hasta que sean recibidos todos los datos
void Inicializar(int loop) {
  // Variable de control para el loop de recepcion de datos
  // Existe una dependencia de la variable loop, si se quiere que espere o no dentro del de un time_off por ejemplo
  boolean leave_loop = true;

  while (leave_loop) {
    // limpiio la variable a cada vez
    DataIn = "";
    // Pregunto por nuevos datos de Entrada desde el serial del Bluetooth
    if (Serial1.available()) {
      //Espera hasta el ultimo dato
      while (Serial1.available()) {
        //Concatena los caracteres de Entrada
        DataIn += (char) Serial1.read();
        //Retardo de tiempo necesario para comunicacion, para no perder datos en el Buffer de entrada
        delay(10);
      }
      // Sale del While leave_loop
      leave_loop = false;
      loop = 0;
      // LLama a la funcion para separar los datos en sus respectivas variables
      //Serial.println("Entro a separar parametros ************************");
      SepararParametros(DataIn, TamBuf);

    } else { // Fi no está disponible el puerto pregunta para salir del While
      if (loop == 1) {
        leave_loop = true;
      } else {
        //Si no hay nuevos datos en esta pregunta de conexion entonces sale del While leave_loop
        leave_loop = false;
      }
    }
  }

}

// Fin Inicializar
/**************************************************************************************/
// Identifica tdos los parametros de estimulacion desde el mensaje de entrada DadaIn

void SepararParametros(String Parametros, int TamBuf) {
  int c = 0;
  Val = "";
  char charBufP[TamBuf]; // --nos valores maximos temos 40
  Parametros.toCharArray(charBufP, TamBuf);//Convierte el Strin a un Array de caracteres (Parametros es el valor DataIn)
  // Serial.println("Caracteres");
  int i = 0; // Capturo el pirmer caracter
  char CharIn = charBufP[i]; // inicio del mensaje

  /********************************************************************************/
  //Aqui separo el mensaje principal
  /********************************************************************************/
  if (CharIn == Ini) { //caracter inicial
    //Serial.println("Entro en el mensaje de los datos de Entrada ************************");
    i = 1; // comiezo con el segundo caracter del  mensaje
    CharIn = charBufP[i];
    while (CharIn != Fin) {//Caracter final
      if (CharIn != csp)  {//Caracter separador
        Val = Val + CharIn;//Concateno los caracteres
      } else {
        //Almacena cada valor sepado por el ";""
        PE[c] = Val;
        Val = "";
        c += 1;//Conta una posicion mas en el arreglo de strings
      }
      i += 1;
      CharIn = charBufP[i];
    }
    // para el ultimo valor
    PE[c] = Val;

    /********************************************************************************/
    //Aqui separo el mensaje de actualizacion de parametros
    /********************************************************************************/

  } else if (CharIn == Ini2) {
    //Serial.println("Entro en el mensaje de Parametro Independienre ************************");
    i = 1; // comiezo con el segundo caracter del  mensaje
    CharIn = charBufP[i];
    while (CharIn != Fin2) {//Caracter final
      if (CharIn != csp)  {//Caracter separador
        Val = Val + CharIn;//Concateno los caracteres
      } else {
        //imprime el numero del index para guardar en la variable de los parametros de estimulacion
        //y actuzliza dicho valor
        //Serial.println("");
        //Serial.print("Numero de Index: ");
        //Serial.println(Val);
        int ValTem = Val.toInt();
        i += 1;
        CharIn = charBufP[i];
        Val = ""; //Limpio para capturar el valor independiente de entrada
        while (CharIn != Fin2) {//Caracter final
          if (CharIn != csp)  {//Caracter separador
            Val = Val + CharIn;//Concateno los caracteres
          }
          i += 1;
          CharIn = charBufP[i];
        }
        //Serial.print("Valor: ");
        //Serial.println(Val);
        //Serial.println("");

        PE[ValTem] = Val;
        SwitchSeparaParametrosIndex(ValTem + 1);

        Val = "";
        i -= 1;
      }
      i += 1;
      CharIn = charBufP[i];
    }
    //Serial.println("Problema de conexion");
  }
  // Separacion y asignacion de los valores en cada varaible

  //Serial.print("Caracter final ");
  //Serial.println(CharIn);
  //delay (10000);

  if (CharIn == Fin) {
    //Serial.println("Separando Parametros Datos de Entrada");
    for (int i = 0; i <= Elementos; i++) {
      SwitchSeparaParametrosIndex(i);
      // Serial.println (PE[i]);
    }
    // end of setup

    //Convirtiendo las unidades de String a Int e boolean y asignando Variables
    //ConvertirUnidadesNumericas();
  }//fin Separacion y asignacion de los valores en cada varaible
}//Fin separar parametrs

//Funcion auxiliar de asigancion de variables dependiendo del Index I
void SwitchSeparaParametrosIndex(int i) {
  switch (i) {
    case 1:
      Time_therapy = PE[0];
      v_Time_therapy = Time_therapy.toInt(); //Valor en mim >> pasar a uS
      v_Time_therapy = v_Time_therapy * seg_min * time_uS;
      break;
    case 2:
      intensity = PE[1];
      v_intensity = intensity.toInt();//Valor en mA
      //Convierto ese valor a decimal del dac
      v_intensity_DAC = map(v_intensity, 1, 100, 1, 2047);
      break;
    case 3:
      freq = PE[2];
      v_freq = freq.toInt(); //Valor en Hz
      v_freq = (1 / v_freq) * time_uS;
      Periodo = v_freq;
      break;
    case 4:
      PWidth = PE[3];
      v_PWidth = PWidth.toInt();//valor en uS
      //Calculo el PW_Off que tiene que ser mayor que 1
      v_PWidth_off = Periodo - (v_PWidth * 2);
      break;
    case 5:
      On_time = PE[4];
      v_On_time = On_time.toInt();//Valor en S
      v_On_time = v_On_time * time_uS;
      break;
    case 6:
      Off_time = PE[5];
      v_Off_time = Off_time.toInt();
      v_Off_time = v_Off_time * time_uS;
      break;
    case 7:
      ramp = PE[6];
      v_ramp = ramp.toInt();
      v_ramp = v_ramp * time_mS;
      break;
    case 8:
      Wf = PE[7];
      break;
    case 9:
      if (PE[8].equals("B")) {
        Pl = true;
      }
      else {
        Pl = false;
      }
      break;
    case 10:
      if (PE[9].equals("n")) {
        Ch1 = true;
        vCh1 = 1;
      }
      else {
        Ch1 = false;
        vCh1 = 0;
      }
      break;
    case 11:
      if (PE[10].equals("n")) {
        Ch2 = true;
        vCh2 = 1;
      }
      else {
        Ch2 = false;
        vCh2 = 0;
      }
      break;
    case 12:
      if (PE[11].equals("n")) {
        Ch3 = true;
        vCh3 = 1;
      }
      else {
        Ch3 = false;
        vCh3 = 0;
      }
      break;
    case 13:
      if (PE[12].equals("n")) {
        Ch4 = true;
        vCh4 = 1;
      }
      else {
        Ch4 = false;
        vCh4 = 0;
      }
      break;
  }

}

// Fin Inicializar
/**************************************************************************************/
// Identifica tdos los parametros de estimulacion desde el mensaje de entrada DadaIn
// Todos los valores de tiempo estan uS
void ConvertirUnidadesNumericas() {
  v_Time_therapy = Time_therapy.toInt(); //Valor en mim >> pasar a uS
  v_Time_therapy = v_Time_therapy * seg_min * time_uS;
  //
  v_intensity = intensity.toInt();//Valor en mA
  //
  v_freq = freq.toInt(); //Valor en Hz
  v_freq = (1 / v_freq) * time_uS;
  //
  v_PWidth = PWidth.toInt();//valor en uS
  //
  v_On_time = On_time.toInt();//Valor en S
  v_On_time = v_On_time * time_uS;
  //
  v_Off_time = Off_time.toInt();
  v_Off_time = v_Off_time * time_uS;
  //
  v_ramp = ramp.toInt();
  v_ramp = v_ramp * time_mS;

}

// Fin SepararParametros
/**************************************************************************************/
// Funcion Auxiliar para imprimir todos los parametros de estimulacion despues de ser separados
//Si quiero imprimir tanto los valores separados (true) y los valores numericos usados en el firmware (false)
void ImprimePE(boolean An_Ar) {
  if (An_Ar == true) {
    Serial.print("Datos de Entrada para Android: ");
    Serial.println(DataIn);
    //
    Serial.print("Tiempo de Terapia: ");
    Serial.println(Time_therapy);
    //
    Serial.print("intensidad: ");
    Serial.println(intensity);
    //
    Serial.print("Frequencia: ");
    Serial.println(freq);
    //
    Serial.print("Largura de Pulso: ");
    Serial.println(PWidth);
    //
    Serial.print("Tiempo On: ");
    Serial.println(On_time);
    //
    Serial.print("Tiempo Off: ");
    Serial.println(On_time);
    //
    Serial.print("Tiempo de Rampa: ");
    Serial.println(ramp);
    //
    if (Wf.equals("S")) {
      Serial.print("Forma de Onda: Quadrada ");
    } else if (Wf.equals("E")) {
      Serial.print("Forma de Onda: Exponencial ");
    } else if (Wf.equals("s")) {
      Serial.print("Forma de Onda: Senoidal ");
    }
    // ----------------
    if (Pl == true) {
      Serial.println("Bifasica");
    } else {
      Serial.println("Monofasica");
    }
    // ----------------
    // Mustro la Habilitacion de los Canales
    if (Ch1 == true) {
      Serial.println("Canal 1 Activado");
    } else {
      Serial.println("Canal 1 Desactivado");
    }
    //
    if (Ch2 == true) {
      Serial.println("Canal 2 Activado");
    } else {
      Serial.println("Canal 2 Desactivado");
    }
    //
    if (Ch3 == true) {
      Serial.println("Canal 3 Activado");
    } else {
      Serial.println("Canal 3 Desactivado");
    }
    //
    if (Ch4 == true) {
      Serial.println("Canal 4 Activado");
    } else {
      Serial.println("Canal 4 Desactivado");
    }
    Serial.println("");
    Serial.println("");
  } else {
    Serial.print("Datos de Entrada para Arduino: ");
    Serial.println(DataIn);
    //
    Serial.print("Tiempo de Terapia: ");
    Serial.println(v_Time_therapy);
    //
    Serial.print("intensidad: ");
    Serial.println(v_intensity);
    //
    Serial.print("Frequencia: ");
    Serial.println(v_freq);
    Serial.print("Periodo: ");
    Serial.println(Periodo);
    //
    Serial.print("Largura de Pulso: ");
    Serial.println(v_PWidth);
    //
    Serial.print("Tiempo On: ");
    Serial.println(v_On_time);
    //
    Serial.print("Tiempo Off: ");
    Serial.println(v_On_time);
    //
    Serial.print("Tiempo de Rampa: ");
    Serial.println(v_ramp);
    //
    if (Wf.equals("S")) {
      Serial.print("Forma de Onda: Quadrada ");
    } else if (Wf.equals("E")) {
      Serial.print("Forma de Onda: Exponencial ");
    } else if (Wf.equals("s")) {
      Serial.print("Forma de Onda: Senoidal ");
    }
    // ----------------
    if (Pl == true) {
      Serial.println("Bifasica");
    } else {
      Serial.println("Monofasica");
    }
    // ----------------
    // Mustro la Habilitacion de los Canales
    if (Ch1 == true) {
      Serial.println("Canal 1 Activado");
    } else {
      Serial.println("Canal 1 Desactivado");
    }
    //
    if (Ch2 == true) {
      Serial.println("Canal 2 Activado");
    } else {
      Serial.println("Canal 2 Desactivado");
    }
    //
    if (Ch3 == true) {
      Serial.println("Canal 3 Activado");
    } else {
      Serial.println("Canal 3 Desactivado");
    }
    //
    if (Ch4 == true) {
      Serial.println("Canal 4 Activado");
    } else {
      Serial.println("Canal 4 Desactivado");
    }
    Serial.println("");
    Serial.println("");
  }

}

// Fin Mostrar datos separados del arreglo de PE
/********************************************************************************************/
/************************** Funciones de Desempeño y control  *******************************/
/********************************************************************************************/
// Cera todas las salidas de los canales y coloca un miliamper en las Precargas
void InhabilitarCanales() {
  //Habilito solo las Precargas
  digitalWrite(RELAY_CH_1, LOW);
  digitalWrite(RELAY_CH_2, LOW);
  digitalWrite(RELAY_CH_3, LOW);
  digitalWrite(RELAY_CH_4, LOW);
  //valor medio de + 20 puntos para crear un pequeño estimulo permanente en la precarga
  for (int ch = 0; ch < 4; ch++) {
    EnviarValorDAC(ch, 1, V_MED + 20);
  }

}

// Se comunica con el dac y actualiza las salidas
void EnviarValorDAC(int address, int operation_mode, uint16_t value) {
  byte valueToWriteH = 0;
  byte valueToWriteL = 0;
  // los canales 2 e 4 estan invertidos en el estimulador - eso se corrige aqui
  if (address == 1) {
    address = 3;
  } else if (address == 3) {
    address = 1;
  }
  valueToWriteH = highByte(value);
  valueToWriteH = 0b00001111 & valueToWriteH;
  valueToWriteH = (address << 6) | (operation_mode << 4) | valueToWriteH;
  valueToWriteL = lowByte(value);
  // Aqui ocurre la magia
  digitalWrite(DAC_SS_PIN, LOW);
  SPI.transfer(valueToWriteH);
  SPI.transfer(valueToWriteL);
  digitalWrite(DAC_SS_PIN, HIGH);

}

void VerificarCanalesActivos() {
  if (Ch1 == true || Ch2 == true || Ch3 == true || Ch4 == true) {
    CanalesActivos = true;
    opStim = (vCh4 << 4) | (vCh3 << 3) | (vCh2 << 2) | vCh1;
    Serial.print("Valor de la suma en binario: ");
    Serial.println(opStim, BIN);
    Serial.print("Valor de la suma en decimal: ");
    Serial.println(opStim, DEC);
    Serial.println(" ");

  } else {
    CanalesActivos = false;
  }


}

// Cera todas las salidas de los canales y coloca un miliamper en las Precargas
void Estimulacion_Start() {
  int c = 0;
  micros_act_th = micros();
  micros_temp_th = micros();
  //Serial.println("Valor inicial micros: ");
  //Serial.println(micros_act);
  VerificarCanalesActivos();


  while (//Se hacen tres comparaciones:
    (unsigned long)(micros_temp_th - micros_act_th) < v_Time_therapy && //Para el tiempo que dura la terapia
    SalidaTh_BtnEmergencia == false && 									//Para el boton de Emergencia
    CanalesActivos == true) {											//Para Saber si hay activacion de canales

    Serial.println("Entro en el while Terapia");
    /************************************************/
    /****** Hago el tiempo de estimulacion On *******/
    /************************************************/
    micros_act_t_on = micros();
    micros_temp_t_on = micros();

    Serial.println("Entrou en el while On Time");
    while (//Se hacen tres comparaciones:
      (unsigned long)(micros_temp_t_on - micros_act_t_on) < v_On_time && 	//Para el tiempo que dura el tiempo On
      CanalesActivos == true && 											//Para Saber si hay activacion de canales
      SalidaTh_BtnEmergencia == false) {									//Para el boton de Emergencia

      //Mando los valores al dac y pregunto por otras cosas como la comunicacion


      /****************************************************************/
      /****** Hago el tiempo de la forma de onda siendo Bipolar *******/
      /****************************************************************/
      if (Pl == true) { //Si es Bipolar
        Serial.println("Entro en el while PW y frequencia de onda Bipolar");
        micros_act_fq = micros();
        micros_temp_fq = micros();
        /////////////////// Tiempo total del Periodo ////////////////////////
        while (
          (unsigned long)(micros_temp_fq - micros_act_fq) < Periodo &&	//Para el tiempo que dura el tiempo On
          CanalesActivos == true && 										//Para el tiempo que dura el tiempo On
          SalidaTh_BtnEmergencia == false) {								//Para el boton de Emergencia

          micros_act_pw = micros();
          micros_temp_pw = micros();

          ///////////////////////////// Pw positiva /////////////////////////////////////////////////////////////////
          while (
            (unsigned long)(micros_temp_pw - micros_act_pw) < v_PWidth &&	//Para el tiempo que dura el tiempo On
            CanalesActivos == true && 										//Para el tiempo que dura el tiempo On
            SalidaTh_BtnEmergencia == false) {								//Para el boton de Emergencia
            //Envio todo al DAC
            //EnviarValorDAC();



            Inicializar(0);//Solo una vez pregunta por nuevos parametros
            VerificarCanalesActivos();//Si aun hay canales activos
            BotonEmergencia();//Si hay una pulsacion del boton de emergencia
            micros_temp_pw = micros();//Actualizo el valor
          }//Fin While PW posotiva

          micros_act_pw = micros();
          micros_temp_pw = micros();
          ///////////////////////////// Pw Negativa //////////////////////////////////////////////////////////////////
          while (
            (unsigned long)(micros_temp_pw - micros_act_pw) < v_PWidth &&	//Para el tiempo que dura el tiempo On
            CanalesActivos == true && 										//Para el tiempo que dura el tiempo On
            SalidaTh_BtnEmergencia == false) {								//Para el boton de Emergencia

            micros_temp_pw = micros(); //Actualizo el valor
          }//Fin While PW posotiva

          //Para Saber si existe un Pw _off y realizar el retardo
          if (v_PWidth_off > 0) {
            micros_act_pw = micros();
            micros_temp_pw = micros();
            ///////////////////////////// Pw positiva ///////////////////////////////////////////////////////////////
            while (
              (unsigned long)(micros_temp_pw - micros_act_pw) < v_PWidth_off &&	//Para el tiempo que dura el tiempo On
              CanalesActivos == true && 										//Para el tiempo que dura el tiempo On
              SalidaTh_BtnEmergencia == false) {								//Para el boton de Emergencia

              micros_temp_pw = micros();//Actualizo el valor
            }//Fin While PW posotiva
          }








          micros_temp_fq = micros(); //Actualizo el valor
        }//Fin While Pw y Frecuencia
      }//Fin if Onda  Bipolar

      /******************************************************************/
      /****** Hago el tiempo de la forma de onda siendo Monopolar *******/
      /******************************************************************/
      else { //Si es monopolar


      }//Fin else Onda Monopolar





      micros_temp_t_on = micros(); //Actualizo el valor
    }
    c += 1;
    Serial.print(c);
    Serial.println(" Seg On");
    //Serial.println("Salió del while On Time");

    /************************************************/
    /****** Hago el tiempo de estimulacion Off ******/
    /************************************************/
    micros_act_t_of = micros();
    micros_temp_t_of = micros();
    //Serial.println("Entrou en el while Off Time");
    while ((unsigned long)(micros_temp_t_of - micros_act_t_of) < v_Off_time && CanalesActivos == true) {
      //Mando los valores al dac y pregunto por otras cosas como la comunicacion
      Inicializar(0);
      VerificarCanalesActivos();
      if (CanalesActivos == true) {
        Serial.println("LLegaron Nuevos Datos, Actualizando.");
        ImprimePE(false);
      }

      micros_temp_t_of = micros();
    }
    c += 1;
    Serial.print(c);
    Serial.println(" Seg Off");
    //Serial.println("Salió del while Off Time");





    micros_temp_th = micros();
  }//Fin While tiempo de terapia

  //Para teste
  if (CanalesActivos == true) {
    Serial.println("Salió del while Terapia");
  } else {
    Serial.println("No hay canales activados");
  }
}//Fin funcion comenzar terapia


// Verifico si el boton de emergencia está activado
void BotonEmergencia() {
  if (digitalRead(STOP_BUTTON)) {
    SalidaTh_BtnEmergencia = true;
  } else {
    SalidaTh_BtnEmergencia = false;
  }
}//Fin funcion Boton de emergencia

void EstimulacionOutputDAC() {

  if (Ch1 == true) {
    Serial.println("Procedimiento para valores de los dac's");

  }

}


