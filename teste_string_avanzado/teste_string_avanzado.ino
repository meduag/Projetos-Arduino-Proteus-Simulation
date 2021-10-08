// Funcionando

char * topic = "/v1.6/devices/my-new-device/aire/lv";

// Coloco los label de las variables que voy a usar
#define V_LABEL1  "luz1"
#define V_LABEL2  "luz2"
#define V_LABEL3  "luz3"
#define V_LABEL4  "tv"
#define V_LABEL5  "aire"

int c = 0;

const uint8_t NUMBER_OF_VARIABLES = 5; // Cantidad de variables a las que el programa se va a suscribir
char * variable_labels[NUMBER_OF_VARIABLES] = {V_LABEL1, V_LABEL2, V_LABEL3, V_LABEL4, V_LABEL5}; // Nombres de las variables

void setup() {
  Serial.begin(115200);
  Serial.println("Teste de serial avanzado!!!");
}

void loop() {
  if (Serial.available() != 0)  {
    if (c == 0) {
      char* variable_label = (char *) malloc(sizeof(char) * 30);
      obtiene_variable_label_topic(topic, variable_label);
      Serial.print("Nuevo valor de variable_label: ");
      Serial.println(variable_label);
      delay(500);
      c = 1;
    }
  }
}

// Como llega un msn con la variable y el valor que cambió, aqui separo esas informaciones del topico
void obtiene_variable_label_topic(char * topic, char * variable_label) {
  sprintf(variable_label, ""); // Convierto variable_label en un array de caracteres para poder compararlo con topic
  Serial.print("Valor de variable_label: ");
  Serial.println(variable_label);
  // Recorro el array de variables

  for (int i = 0; i < NUMBER_OF_VARIABLES; i++) {
    Serial.print(i);
    Serial.print(" Label: ");
    Serial.println(variable_labels[i]);

  }

  Serial.println("");

  for (int i = 0; i < NUMBER_OF_VARIABLES; i++) {
    // comparo el array para saber que parte esta igual deacuerdo las variables que defini

    char * result_lv = strstr(topic, variable_labels[i]);
    char * rslv = result_lv;

    Serial.print(i);
    Serial.print(" Label: ");
    Serial.println(variable_labels[i]);

    Serial.print(i);
    Serial.print(" Topic: ");
    Serial.println(topic);

    Serial.print("Valor de result_tv: ");
    Serial.println(rslv);// Imprimo para ver el resultado

    /*String strt((char*)topic);
      String strv((char*)variable_labels[i]);
      int comp = strv.compareTo(strt);

      Serial.print("Valor de comparacion: ");
      Serial.println(comp);// Imprimo para ver el resultado*/

    //if (result_lv != NULL) {
    if (strstr(topic, variable_labels[i])) {
      Serial.println("result_lv NO!! es nulo");
      uint8_t len = strlen(result_lv);
      char result[100];
      uint8_t i = 0;
      for (i = 0; i < len - 3; i++) {
        result[i] = result_lv[i];
      }
      result[i] = '\0';
      sprintf(variable_label, "%s", result);
      break;
    } else {
      Serial.println("result_lv ES!! nulo \n\n");
    }

  }
}
