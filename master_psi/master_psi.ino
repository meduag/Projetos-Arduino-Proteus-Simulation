#include <SPI.h>  // include the SPI library:
uint8_t counter = 0;
uint8_t zero = 0;
const int slaveSelectPin = 10;

void setup() {
  Serial.begin(115200);
  delay(1000);
  // set the slaveSelectPin as an output:
  pinMode (slaveSelectPin, OUTPUT);
  // initialize SPI:
  SPI.begin(); 
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
}

void loop() {
  receiveShit();
}

int receiveShit() {
  uint16_t data;
  // take the SS pin low to select the chip:
  digitalWrite(slaveSelectPin,LOW);
  
  //  send in the address and value via SPI:
  data = SPI.transfer(zero);
  
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin,HIGH);
   
  Serial.print(data);
  Serial.println(" .. ");

  delay(1000);
  /*counter = counter + 1;
  if (counter == 255) {
    Serial.println("");
    counter = 0;
  }*/

}
