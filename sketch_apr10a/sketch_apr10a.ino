int led = 13;
int n1 = 1;
int n2 = 2;
int res = 0;

int time2 = 0;
int contar_tempo = 1000;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  //com_delay();
  com_millis();
}

void com_delay() {
  digitalWrite(led, HIGH);
  delay(1000);
  res = res + n1 + (n2 * 2);
  Serial.print("Resposta = ");
  Serial.println(res);
  digitalWrite(led, LOW);
  delay(1000);
}

void com_millis() {
  //Serial.print("Millis = ");
  //Serial.println(millis());
  //delay(1000);
  if (millis() > time2 + contar_tempo) { // ¿Ha transcurrido el periodo?
    Serial.print("Millis = ");
    Serial.println(millis());
    time2 = millis();
  }






}
