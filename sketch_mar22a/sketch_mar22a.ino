int box = 0;

void setup() {
  pinMode(8, INPUT);
  pinMode(13, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  //box é uma caixinha onde guardo as coisa da contagem

  Serial.println(box);

  if (box <= 3) {
    digitalWrite(13, 0);
    delay(1000);
    digitalWrite(13, 1);
    delay(1000);
    digitalWrite(13, 0);
  }

  if (box >= 4) {
    box = 4;
  } else {
    box = box + 1;
  }
}
