void setup() {
  Serial.begin(9600);

}

void loop() {
  for (int i = 0; i < 10; i++) {
    Serial.print("Valor ");
    Serial.println(i);
    delay(1000);
  }

}
