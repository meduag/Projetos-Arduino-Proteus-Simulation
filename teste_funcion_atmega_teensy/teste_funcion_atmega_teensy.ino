char * var_lb = "tv";
float value_in = 1;

int c = 0;

#define V_LABEL1  "luz1"
#define V_LABEL2  "luz2"
#define V_LABEL3  "luz3"
#define V_LABEL4  "tv"
#define V_LABEL5  "aire"

const uint8_t NUMBER_OF_VARIABLES = 5; // Cantidad de variables a las que el programa se va a suscribir
char * variable_labels[NUMBER_OF_VARIABLES] = {V_LABEL1, V_LABEL2, V_LABEL3, V_LABEL4, V_LABEL5};

// Variables para datos de salida
String data_out; // Variable general para comunicacion serial
const char fin = '>'; // Caracter separador

// Variables globales para activacion
/************* BOOL *******************/
// Pos Array  Descripcion   Activacion    Data_out
// 0          Light 1       on/off        0/1
// 1          Light 2       on/off        0/1
// 2          Light 3       on/off        0/1
// 3          Tv            on/off        0/1
// 4          Air           on/off        0/1
bool b_var[] = { 0, 0, 0, 0, 0};

String msn_base = "0>0>0>0>0>0>0>";

bool ctrl_while = true;

void setup() {
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (c == 0) {
    SerialPrint_ATMEGA8(value_in, var_lb);
  }

}

//Funcion que ejecuta las acciones en el ATMEGA8/Teensy 3.0
void SerialPrint_ATMEGA8(float value_in, char * variable_label_in) {
  //Variable auxiliar comprobar label numerica
  int comp_lb_num;
  // Primero asigno un valor numerico a cada label variable
  int i = 0;
  while (i < NUMBER_OF_VARIABLES) { // para quitar los tres ultimos valores
    Serial.print("Numero de Variable a comparar: ");
    Serial.println(i);
    Serial.println("");

    if (strcmp(variable_labels[i], variable_label_in) == 0) {
      Serial.println("Asignando valor a comp_lb_num");
      comp_lb_num = i;
      i = NUMBER_OF_VARIABLES;
    } else {
      i++;
    }
    delay(2000);
  }

  Serial.print("Numero de Variable: ");
  Serial.print(comp_lb_num);
  Serial.print(" >>> Label de la Variable: ");
  Serial.print(variable_labels[comp_lb_num]);
  c = 1;

  //Separa_concatena_msn(value_in, comp_lb_num);
}
