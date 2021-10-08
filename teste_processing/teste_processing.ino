const int pin_Led = 2;

void setup() {
  Serial.begin(115200);
  pinMode(pin_Led, OUTPUT);
}

void loop() {
  if(Serial.available()){
    char sel = Serial.read();
    
    if(sel == '1'){
      digitalWrite(pin_Led, HIGH);
      Serial.write('0');
    }

    if(sel == '0'){
      digitalWrite(pin_Led, LOW);
      Serial.write('1');
    }
  }

}
