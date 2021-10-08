const int pin_pulso1 = 3;
const int pin_pulso2 = 4;

String data_in = "n";

int pulso1 = 0;
int pulso2 = 0;
int sel = 0;

const char fin = '>';

void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200);
  pinMode(pin_pulso1, INPUT_PULLUP);
  pinMode(pin_pulso2, INPUT_PULLUP);


}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() != 0) {
    data_in = Serial.readStringUntil(fin);
    sel = data_in.toInt();
  }

  if (sel == 1) {
    pulso1 = digitalRead(pin_pulso1);
    pulso2 = digitalRead(pin_pulso2);

    Serial.print(pulso1 * 5);
    Serial.print("\t");
    Serial.println(pulso2 * 5);
    delay(10);
  }
}
