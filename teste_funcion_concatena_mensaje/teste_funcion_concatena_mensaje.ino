float vin = 3.325;
String msn_base = "0>0>0>0>0>";
int c = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Confirmar funcion");

}

void loop() {
  if (c == 0) {
    Serial.print("Mensagem original: ");
    Serial.println(msn_base);
    Separa_concatena_msn(vin, 1);
    Serial.print("Mensagem modificada: ");
    Serial.println(msn_base);
    c = 1;
  }

}

void Separa_concatena_msn(float value_in, int label_n) {
  // "0>0>0>0>0>"; Mensaje de ejemplo
  
  // convierto de float a entero
  int tvi = value_in; 

  // convierto de entero a caracter llevando en consderacion el codigo ascii
  char up_ve = tvi + 48; 

  // Actualizo el mensaje que voy a enviar al ATMEGA/TEENSY
  switch (label_n) {
    case 0:
      // luz1
      msn_base.setCharAt(0, up_ve);
      break;
    case 1:
      // luz2
      msn_base.setCharAt(2, up_ve);
      break;
    case 2:
      // luz3
      msn_base.setCharAt(4, up_ve);
      break;
    case 3:
      // Tv
      msn_base.setCharAt(6, up_ve);
      break;
    case 4:
      // Aire
      msn_base.setCharAt(8, up_ve);
      break;
  }
}
