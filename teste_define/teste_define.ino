#define PIN_CS 13
#define PIN_CS_ENA pinMode(PIN_CS, OUTPUT)
#define PIN_CS_LOW digitalWrite(PIN_CS,LOW)
#define PIN_CS_HIGH digitalWrite(PIN_CS, HIGH)
#define TIME1S delay(1000)

void setup() {
  PIN_CS_ENA;
  PIN_CS_LOW;
}

void loop() {
  PIN_CS_HIGH;
  TIME1S;
  PIN_CS_LOW;
  TIME1S;
}
