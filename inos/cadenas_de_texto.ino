
char* Menu_Pro[] = {"Electro Diagnost", "Terapeutica     "};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  String Cadena = Menu_Pro[1];
  Serial.println(Cadena);//IMPRIMIMOS DE QUE HEMOS PEDIDO EL MESNAJE AL DISPOSITIVO EN EL CANAL 1
  Cadena = Menu_Pro[1];
  Serial.println(Cadena);//IMPRIMIMOS DE QUE HEMOS PEDIDO EL MESNAJE AL DISPOSITIVO EN EL CANAL 1


}
