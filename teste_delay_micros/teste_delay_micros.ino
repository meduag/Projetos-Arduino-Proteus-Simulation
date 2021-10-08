// teste millis();

unsigned long tempo_atual = 0;
unsigned long tempo_final = 1000;

#define led 13

void setup() {
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
}

void loop() {
  tempo_atual = micros();
  tempo_final = tempo_atual + tempo_final;

  while(micros() < tempo_final) {
    digitalWrite(led, HIGH);
  }
  tempo_final = 1000000;
  digitalWrite(led, LOW);
  delay(1000);
}
