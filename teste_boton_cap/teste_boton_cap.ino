int PWM = 5;

int IN1 = 2;
int IN2 = 3;
int LED = 13;

int valA0 = 0;

bool act = false;


void setup() {
  //Serial.begin(115200);

  pinMode(A0, INPUT);

  pinMode(PWM, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(LED, LOW);

}

void loop() {

  valA0 = analogRead(A0);
  if (valA0 > 500) {
    while (valA0 > 500) {
      digitalWrite(LED, HIGH);
      delay(70);
      digitalWrite(LED, LOW);
      delay(70);
      valA0 = analogRead(A0);
    }
    act = !act;
  }

  if (act == true) {
    analogWrite(PWM, 128);
    digitalWrite(IN1, HIGH);
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(LED, LOW);
  }
}
