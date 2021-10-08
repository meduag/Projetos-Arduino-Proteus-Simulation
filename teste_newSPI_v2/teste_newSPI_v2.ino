#include <Arduino.h>
#include <spi4teensy3.h>

const int chipselect = 10;
const int num_register = 4;

unsigned long regVal = 0;

const int pin_RESET = 9;

#define  ADAS1000_CMREFCTL   0x05  /* Common Mode Reference and Shield Drive Control Register */

void setup() {
  pinMode(chipselect, OUTPUT);
  digitalWrite(chipselect, HIGH); // ~CE is active LOW, so this disables shifting

  pinMode(pin_RESET, OUTPUT);
  digitalWrite(pin_RESET, HIGH); // ~CE is active LOW, so this disables shifting

  spi4teensy3::init(1, 0, 0 );
  Serial.begin(115200);

  // reset adas
  delay(500);
  digitalWrite(pin_RESET, LOW); // ~CE is active LOW, so this disables shifting
  delay(500);
  digitalWrite(pin_RESET, HIGH); // ~CE is active LOW, so this disables shifting
}

void loop() {
  ADAS1000_SetRegisterValue(ADAS1000_CMREFCTL, 0x85E0000A);
  delay(1000);
  ADAS1000_GetRegisterValue(ADAS1000_CMREFCTL, &regVal);
  delay(1000);
}

void ADAS1000_SetRegisterValue(unsigned char regAddress, unsigned long regVal) {
  unsigned char writeCmd[4] = {0, 0, 0, 0};

  writeCmd[0] = 0x80 + regAddress;  // Write bit and register address.
  writeCmd[1] = (unsigned char)((regVal & 0xFF0000) >> 16);
  writeCmd[2] = (unsigned char)((regVal & 0x00FF00) >> 8);
  writeCmd[3] = (unsigned char)((regVal & 0x0000FF) >> 0);

  Serial.println("Data for Send to Write SPI");
  byte i;
  for (i = 0; i < num_register; i++) {
    Serial.print("writeCmd[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(writeCmd[i], BIN);
  }

  digitalWrite(chipselect, LOW); // enable shifting
  //delayMicroseconds(1); // give the chips some time to do their job
  spi4teensy3::send(writeCmd, num_register);
  digitalWrite(chipselect, HIGH); // disable clock again*/
}

void ADAS1000_GetRegisterValue(unsigned char regAddress, unsigned long* regVal) {
  unsigned char readCmd[num_register]  = {0, 0, 0, 0};
  unsigned char readDataIn[num_register] = {0, 0, 0, 0};

  // Select the register (For register reads, data is shifted out
  // during the next word).
  readCmd[0] = regAddress;  // Register address.
  Serial.println("Data for Send to Read SPI");
  byte i;
  for (i = 0; i < num_register; i++) {
    Serial.print("readCmd[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(readCmd[i], BIN);
  }

  // Write de command
  digitalWrite(chipselect, LOW); // enable shifting
  spi4teensy3::send(readCmd, num_register);
  digitalWrite(chipselect, HIGH); // disable clock again*/

  // Read the data from the device.
  digitalWrite(chipselect, LOW); // enable shifting
  spi4teensy3::receive(readDataIn, num_register);
  digitalWrite(chipselect, HIGH); // disable clock again*/

  *regVal = ((unsigned long)readDataIn[1] << 16) +
            ((unsigned long)readDataIn[2] << 8) +
            ((unsigned long)readDataIn[3] << 0);

  Serial.println("Data Reading from SPI");
  for (i = 0; i < num_register; i++) {
    Serial.print("readDataIn[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(readDataIn[i], BIN);
  }

}
