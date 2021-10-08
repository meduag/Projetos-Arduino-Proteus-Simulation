//canal 1
int c1_1 = 0, c1_2 = 1, c1_3 = 2;

//canal 2
int c2_1 = 3, c2_2 = 4, c2_3 = 5;

//canal 3
int c3_1 = 6, c3_2 = 7, c3_3 = 8;

//canal 4
int c4_1 = 9, c4_2 = 10, c4_3 = 11;

// SAIDAS
int sc1 = 20 , sc2 = 21, sc3 = 22, sc4 = 23;



void setup() {
  // canal 1
  pinMode(c1_1, INPUT_PULLUP);
  pinMode(c1_2, INPUT_PULLUP);
  pinMode(c1_3, INPUT_PULLUP);

  // canal 2
  pinMode(c2_1, INPUT_PULLUP);
  pinMode(c2_2, INPUT_PULLUP);
  pinMode(c2_3, INPUT_PULLUP);

  // canal 3
  pinMode(c3_1, INPUT_PULLUP);
  pinMode(c3_2, INPUT_PULLUP);
  pinMode(c3_3, INPUT_PULLUP);

  //canal 4
  pinMode(c4_1, INPUT_PULLUP);
  pinMode(c4_2, INPUT_PULLUP);
  pinMode(c4_3, INPUT_PULLUP);

  //saidas
  pinMode(sc1, OUTPUT);
  pinMode(sc2, OUTPUT);
  pinMode(sc3, OUTPUT);
  pinMode(sc4, OUTPUT);
}

void loop() {

  if (digitalRead(c1_1) == 1 &&  digitalRead(c1_2) == 1 && digitalRead(c1_3) == 1) {
    digitalWrite(sc1, HIGH);
  } else {
    digitalWrite(sc1, LOW);
  }

  if (digitalRead(c2_1) == 1 &&  digitalRead(c2_2) == 1 && digitalRead(c2_3) == 1) {
    digitalWrite(sc2, HIGH);
  } else {
    digitalWrite(sc2, LOW);
  }

  if (digitalRead(c3_1) == 1 &&  digitalRead(c3_2) == 1 && digitalRead(c3_3) == 1) {
    digitalWrite(sc3, HIGH);
  } else {
    digitalWrite(sc3, LOW);
  }

  if (digitalRead(c4_1) == 1 &&  digitalRead(c4_2) == 1 && digitalRead(c4_3) == 1) {
    digitalWrite(sc4, HIGH);
  } else {
    digitalWrite(sc4, LOW);
  }

}
