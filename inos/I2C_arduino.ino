#include <Wire.h>

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
}

void loop() {
  //for (int i = 0; i <= 10; i++) {
  byte ws = 1;
  Wire.beginTransmission(0);
  Wire.write(ws); // sends x
  delay(100);
  Wire.write(ws); // sends x
  Wire.endTransmission(); // stop transmitting
  delay(500);
  //}


}
