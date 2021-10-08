void setup() {
  Serial.begin(9600);// INICIAMOS EL SERIAL PARA MOSTRAR LOS MENSAJES
}

void loop() {
  lee_botones();
}//fin void loop

void lee_botones() {
  int in1 = analogRead(A5);
  int in2 = analogRead(A6);
  int in3 = analogRead(A7);

  Serial.print("A: ");
  Serial.print(in1 - 1023);

  Serial.print("\tB: ");
  Serial.print(in2 + 1023);

  Serial.print("\tC: ");
  Serial.println(in3);
  delay(10);
}
