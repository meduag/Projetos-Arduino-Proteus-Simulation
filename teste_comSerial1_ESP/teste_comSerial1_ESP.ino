void setup() {
  Serial.begin(115200);
  Serial.setTimeout(100);

  Serial1.begin(115200);
  Serial1.setTimeout(100);

}

void loop() {
  if (Serial1.available() != 0) {
    String data = Serial1.readString();
    Serial.println(data);
  }
}
