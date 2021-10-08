#include <Arduino.h>
#include <spi4teensy3.h>

const byte chipselect = 9;
const byte num_register = 4;

unsigned long regVal = 0;

#define  ADAS1000_CMREFCTL   0x05  /* Common Mode Reference and Shield Drive Control Register */

void setup() {
  pinMode(chipselect, OUTPUT);
  digitalWrite(chipselect, HIGH); // ~CE is active LOW, so this disables shifting
  spi4teensy3::init(0, 0, 0 );
  Serial.begin(115200);
}

void loop() {
  /*uint8_t inval[num_register]; // buffer variable to store the read data
    digitalWrite(chipselect, LOW); // enable shifting
    delayMicroseconds(1); // give the chips some time to do their job
    spi4teensy3::receive(inval, num_register); // read in the values rom the register chain
    digitalWrite(chipselect, HIGH); // disable clock again

    digitalWrite(chipselect, LOW); // enable shifting
    delayMicroseconds(1); // give the chips some time to do their job
    spi4teensy3::send(inval, num_register); // read in the values rom the register chain
    digitalWrite(chipselect, HIGH); // disable clock again*/

  ADAS1000_SetRegisterValue(ADAS1000_CMREFCTL, 0x85E0000B);
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

  Serial.println("Write SPI");
  byte i;
  for (i = 0; i < num_register; i++) {
    Serial.print("writeCmd[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(writeCmd[i], BIN);
  }

  digitalWrite(chipselect, LOW); // enable shifting
  delayMicroseconds(1); // give the chips some time to do their job
  spi4teensy3::send(writeCmd, num_register);
  digitalWrite(chipselect, HIGH); // disable clock again*/
}

void ADAS1000_GetRegisterValue(unsigned char regAddress, unsigned long* regVal) {
  unsigned char readCmd[num_register]  = {0, 0, 0, 0};
  unsigned char readDataIn[num_register] = {0, 0, 0, 0};

  // Select the register (For register reads, data is shifted out
  // during the next word).
  readCmd[0] = regAddress;  // Register address.
  Serial.println("Send to Read SPI");
  byte i;
  for (i = 0; i < num_register; i++) {
    Serial.print("readCmd[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(readCmd[i], BIN);
  }
  spi4teensy3::send(readCmd, num_register);

  // Read the data from the device.
  digitalWrite(chipselect, LOW); // enable shifting
  delayMicroseconds(1); // give the chips some time to do their job
  spi4teensy3::receive(readDataIn, num_register);
  digitalWrite(chipselect, HIGH); // disable clock again*/

  *regVal = ((unsigned long)readDataIn[1] << 16) +
            ((unsigned long)readDataIn[2] << 8) +
            ((unsigned long)readDataIn[3] << 0);

  Serial.println("Read SPI");

  for (i = 0; i < num_register; i++) {
    Serial.print("readDataIn[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(readDataIn[i], BIN);
  }

}
