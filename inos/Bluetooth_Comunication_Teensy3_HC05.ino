#include <SoftwareSerial.h>

#define RxD 1
#define TxD 0

String in = "";

SoftwareSerial BTS(RxD, TxD);

void setup() {
  Serial.begin(9600);

  BTS.flush();
  delay(500);
  BTS.begin(9600);

  Serial.println("Comandos AT");
  delay(1000);

}

void loop() {
  
  if (BTS.available()) {
    while (BTS.available()) {
      in += (char)BTS.read();
    }
  }

  //Serial.println(in);

  if (Serial.available()) {
    delay(10);
    Serial.println(Serial.read());
    BTS.write(Serial.read());
    
  }
}
