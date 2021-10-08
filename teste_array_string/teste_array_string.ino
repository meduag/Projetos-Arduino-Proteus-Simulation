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
String v_labels[NUMBER_OF_VARIABLES] = {V_LABEL1, V_LABEL2, V_LABEL3, V_LABEL4, V_LABEL5};

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("En String");
  for (int i = 0; i < NUMBER_OF_VARIABLES; i++) {
    Serial.println(v_labels[i]);//Insert the dataSource and Variable's Labels
    delay(500);
  }

  Serial.println("En Char");
  for (int i = 0; i < NUMBER_OF_VARIABLES; i++) {
    Serial.println(variable_labels[i]);//Insert the dataSource and Variable's Labels
    delay(500);
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
