#define led 13

unsigned long ta = 0;
unsigned long tf = 1000000;

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
}

void loop() {
  if (Serial.available()) {
    char data = Serial.read();
    Serial.readString(); // Tira o resto do buffer  - hace un flush forzado

    Serial.print("Valor que chegou: ");
    Serial.println(data);
    digitalWrite(led, HIGH);

    if (data == '1') {
      ta = micros();
      tf = ta + tf;
      
      Serial.print("ta: ");
      Serial.print(ta);
      Serial.print("\ttf: ");
      Serial.println(tf);
      
      boolean sal = false;
      Serial.println("Entrou no while!!!");
      while (sal == false) {
        if (micros() > tf) {
          sal = true;
          tf = 1000000;
        }
        Serial.write(micros(), 8);
        Serial.println();
      }
      Serial.println("Saiu no while!!!");
      digitalWrite(led, LOW);
    }

  }

}
