void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(map(0, 0, 100, 200, 400));
  Serial.println(map(25, 0, 100, 200, 400));
  Serial.println(map(50, 0, 100, 200, 400));
  Serial.println(map(75, 0, 100, 200, 400));
  Serial.println(map(100, 0, 100, 200, 400));
  /*El valor puede estar fuera de los rangos,
  pero igualmente se mapea de la forma conveniente */
  Serial.println(map(500, 0, 100, 200, 400));
  delay(2000);

}
