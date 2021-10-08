void setup() {
 Serial.begin(2000000);
 pinMode(A0, INPUT);

}

void loop() {
  double val = analogRead(A0);
  Serial.println(val);
}
