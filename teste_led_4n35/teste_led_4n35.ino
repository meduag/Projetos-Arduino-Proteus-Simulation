int led = 13;
int le = 2;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(le, INPUT_PULLUP);

  digitalWrite(led, LOW);
}

void loop() {
  if(digitalRead(le) == 1){
    digitalWrite(led, HIGH);
  }else{
    digitalWrite(led, LOW);
  }
  
}
