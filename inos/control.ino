void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValue = analogRead(A0);
  int sValue = map(sensorValue, 0,1023,0,100);
  // print out the value you read:
  Serial.println(sValue);
  delay(1);        // delay in between reads for stability
}
