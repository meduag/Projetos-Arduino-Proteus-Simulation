#define PinInt1 23
int estado = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PinInt1, INPUT); // sets the digital pin as output
  attachInterrupt(PinInt1, isrService, CHANGE); // interrrupt 1 is data ready
}

void loop() {
  estado = 0;
  while (true) {
    delay(1000);
    Serial.println("Running");
  }


}

// watermark generates this interrupt
void isrService() {
  //cli();
  Serial.println("At ISR0");
  delay(100);
  estado = 1;
  //sei();
}


/*int pin = 13;
int pin2 = 14;
//volatile int state = LOW;
int estado = 0;

void setup() {
  pinMode(pin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(pin2), blink, CHANGE);
}

void loop() {
  while (estado == 0) {
    digitalWrite(pin, HIGH);
  }

  digitalWrite(pin, LOW);

}

void blink() {
  //state = !state;
  estado = 1;
}*/
