void setup() {
  Serial.begin(9600);

}

void loop() {
  char c[1];
  c[0] = Serial.read();

  switch (c[0]) {
    case 'E':
      Serial.println("Letra E");
      break;
    case '0':
      Serial.println("Valor: 0");
      break;
    case '1':
      Serial.println("Valor: 1");
      break;
    case '2':
      Serial.println("Valor: 2");
      break;
    case '3':
      Serial.println("Valor: 3");
      break;
  }

}
