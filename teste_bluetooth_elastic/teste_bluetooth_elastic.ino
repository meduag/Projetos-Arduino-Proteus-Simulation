int su = 0;
String di = "";
char f = '.';

int l = 13;

bool s = false;

void setup() {
  Serial.begin(9600);
  pinMode(l, OUTPUT);
  digitalWrite(l, LOW);
}

void loop() {
  if(su == 0){
    ble_rename();
    su = 1;
  }

  if (Serial.available()) {
      di = Serial.readStringUntil(f);
      
      if(di == "1"){
        s = !s;
        digitalWrite(l, s);
      }
      
  }

}

void ble_rename() {
  char ID1[20] = "AT+NAMELUCES FORA  ";

/*  Serial.write("AT+RESET");
  Serial.println();
  delay(500); //Bluetooth original

  Serial.write("AT+SAVE1");
  Serial.println();
  delay(500); //Bluetooth original*/

  Serial.write(ID1);
  Serial.println();
  delay(500); //Bluetooth original

}
