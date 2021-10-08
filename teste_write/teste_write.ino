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

    if (data == '1') {
      for (int i = 0; i < 128; i++) {
        Serial.write(i);
      }
    }

  }

}
