#include <UbidotsESPMQTT.h>

// Defino mis puntos de conexion
#define TOKEN "BBFF-THEyBBLGfTxR4xaGrar3EraxqDNO6A" // Tu token de Ubidots
#define WIFINAME "Baeza sala" // Nombre de la red en que se va a conectar
#define WIFIPASS "andrea76" // Password de la red
#define DEVICE_LABEL  "my-new-device"  // Coloco el label de mi device que quiero apuntar

// Coloco los label de las variables que voy a usar
#define V_LABEL1  "luz1"
#define V_LABEL2  "luz2"
#define V_LABEL3  "luz3"
#define V_LABEL4  "tv"
#define V_LABEL5  "aire"

/************* BOOL *******************/
// Pos Array/Dec    Desciption    Activation    Data_in Rx/Tx (>)
// 0/1              Light 1       on/off        0/1
// 1/2              Light 2       on/off        0/1
// 2/3              Light 3       on/off        0/1
// 3/4              Tv            on/off        0/1
// 4/5              Air           on/off        0/1
const uint8_t NUMBER_OF_VARIABLES = 5; // Cantidad de variables a las que el programa se va a suscribir
char * variable_labels[NUMBER_OF_VARIABLES] = {V_LABEL1, V_LABEL2, V_LABEL3, V_LABEL4, V_LABEL5}; // Nombres de las variables
//String v_labels[NUMBER_OF_VARIABLES] = {V_LABEL1, V_LABEL2, V_LABEL3, V_LABEL4, V_LABEL5};

float value; // Variable para almacenar el dato que llega en el payload.

// Variables para datos de salida
String data_out; // Variable general para comunicacion serial

// Variables globales para activacion
/************* BOOL *******************/
// Pos Array  Descripcion   Activacion    Data_out
// 0          Light 1       on/off        0/1
// 1          Light 2       on/off        0/1
// 2          Light 3       on/off        0/1
// 3          Tv            on/off        0/1
// 4          Air           on/off        0/1
int b_var[] = { 0, 0, 0, 0, 0};

// Variables para datos de entrada
String data_in; // General variable for data in serial communication
const char fin = '>'; // Character separator used for split data '>'
int ok = 0; // para saber si esta todo bien con el mensaje

// Mensaje base
String msn_base = "0>0>0>0>0>";

int legnth_bvar = 0;

// Los pines de activacion seran por el ATMEGA 8
Ubidots client(TOKEN);

// Setup
void setup() {
  // put your setup code here, to run once:
  client.ubidotsSetBroker("business.api.ubidots.com"); // Sets the broker properly for the business account
  client.setDebug(false); // Pass a true or false bool value to activate debug messages

  Serial.begin(115200);
  Serial.setTimeout(100);

  client.wifiConnection(WIFINAME, WIFIPASS);
  
  client.begin(callback);
  
  for (int i = 0; i < NUMBER_OF_VARIABLES; i++) {
    client.ubidotsSubscribe(DEVICE_LABEL,  variable_labels[i]);//Insert the dataSource and Variable's Labels
  }

  legnth_bvar = (sizeof(b_var) / sizeof(b_var[0]));
}


// Loop
void loop() {

  if (!client.connected()) {
    client.reconnect();
    for (int i = 0; i < NUMBER_OF_VARIABLES; i++) {
      client.ubidotsSubscribe(DEVICE_LABEL, variable_labels[i]);//Insert the dataSource and Variable's Labels
    }
  }

  if (Serial.available() != 0) {
    //Serial.println("Separando valores");
    for (int i = 0; i < legnth_bvar - 2; i++) {
      data_in = Serial.readStringUntil(fin);
      //Serial.println(data_in);
      
      if (data_in != "1") {
        if (data_in != "0") {
          //Serial.println("Error en el mensaje");
          ok = 0;
        } else {
          ok = 1;
        }
      } else {
        ok = 1;
      }

      if (ok == 1) {
        // Solo guarda estados que esten verificadas
        b_var[i] = data_in.toInt();
        // actualiza el mensaje base
        msn_base.setCharAt(i * 2, data_in.charAt(0));
        //Serial.println(msn_base);
      }
    }

    Serial.readString(); // Read the rest of the message if has
  }

  if (ok == 1) {
    //Serial.println("Actualizando Ubidots");
    for (int i = 0; i < legnth_bvar; i++) {
      float v_send = b_var[i] / 1;
      
     client.add(variable_labels[i], v_send );
     client.ubidotsPublish("my-new-device");
    }
    ok = 0;
  }

  client.loop();
}

/****************************************
   Funciones auxiliares
 ****************************************/
// esta funcion es propietaria y actualiza cada vez que llega un valor
void callback(char* topic, byte* payload, unsigned int length) { // Funcion de callback con llegada de datos
  char* variable_label = (char *) malloc(sizeof(char) * 30); // Aloco memoria para recibir msn de entrada
  obtiene_variable_label_topic(topic, variable_label); // Funcion que identifica el lable de la variable y el valor
  value = byte_to_float(payload, length);

  // Funcion que monta el mensaje para envia via TXT al ATMEGA8
  SerialPrint_ATMEGA8(value, variable_label);

  free(variable_label); // Libero la memoria usada por la variable
}

// Como llega un msn con la variable y el valor que cambió, aqui separo esas informaciones del topico y veo que variable cambió
void obtiene_variable_label_topic(char * topic, char * variable_label) {
  sprintf(variable_label, ""); // Convierto variable_label en un array de caracteres para poder compararlo con topic

  // Recorro el array de variables
  for (int i = 0; i < NUMBER_OF_VARIABLES; i++) {
    // comparo el array para saber que parte esta igual deacuerdo las variables que definí
    // si existe en el topico un avlor igual, el resultado de la comparacion no es Null (Ver strstr)
    char * result_lv = strstr(topic, variable_labels[i]);
    if (strstr(topic, variable_labels[i])) {
      uint8_t len = strlen(result_lv);
      char result[100];
      uint8_t i = 0;
      for (i = 0; i < len - 3; i++) { // para quitar los tres ultimos valores
        result[i] = result_lv[i];
      }
      result[i] = '\0'; // agrego un carater de finalizacion y con eso se convierte en String
      sprintf(variable_label, "%s", result); // Concateno los valores y o guardo en la nueva variable
      break;// Salgo del if con el label identificado
    }
  }
}

// Como o payload esta en BYTE lo convierto a flotante
float byte_to_float(byte * payload, unsigned int length) {
  char * temp = (char *) malloc(sizeof(char) * 10); // aloco memoria
  // recorro los bytes y los convierto a char
  for (int i = 0; i < length; i++) {
    temp[i] = payload[i];
  }
  // convierto de char a float
  float valuet = atof(temp);
  free(temp); //libero memoria
  return valuet; // retorno el valor encontrado como un flotante
}

//Funcion que ejecuta las acciones en el ATMEGA8/Teensy 3.0
void SerialPrint_ATMEGA8(float value_in, char * variable_label_in) {
  //Variable auxiliar comprobar label numerica
  int comp_lb_num;
  // Primero asigno un valor numerico a cada label variable
  int i = 0;
  while (i < NUMBER_OF_VARIABLES) { // para quitar los tres ultimos valores
    if (strcmp(variable_labels[i], variable_label_in) == 0) {
      //Serial.println("Asignando valor a comp_lb_num");
      comp_lb_num = i;

      // convierto de float a entero
      int tvi = value_in;
      
      // convierto de entero a caracter llevando en consderacion el codigo ascii
      char up_ve = tvi + 48;
      msn_base.setCharAt(comp_lb_num * 2, up_ve);

      i = NUMBER_OF_VARIABLES; // Sale del while
    } else {
      i++;
    }
  }
  // Enviar mensaje ATMEGA/Teensy 3.0 puede ser via Serial1
  Serial.println(msn_base);
}
