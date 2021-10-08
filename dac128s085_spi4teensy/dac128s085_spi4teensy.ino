#include <Arduino.h>
#include <spi4teensy3.h>

const byte chipselect = 10;
const byte num_register = 2;
int f = 1;

void setup() {
  pinMode(chipselect, OUTPUT);
  digitalWrite(chipselect, HIGH); // ~CE is active LOW, so this disables shifting
  spi4teensy3::init(0, 0, 0 );
  Serial.begin(115200);
  delay(2000);
}

void loop() {
  if (f == 1) {
    ModoWRT();
    f = 0;
    delay(1000);
  }

  SPI_Write(1, 4095);
  delay(5000);
  SPI_Write(1, 0);
  delay(5000);
}

void SPI_Write(int channel, uint16_t value) {
  unsigned char writeCmd[num_register] = {0, 0};

  writeCmd[0] = highByte(value);
  writeCmd[0] = 0b00001111 & writeCmd[0];
  writeCmd[0] = (channel << 4) | writeCmd[0];
  writeCmd[1] = lowByte(value);

  Serial.println("Update Canal");
  SPI_send(writeCmd);

  Serial.println("Update Salida del canal");
  writeCmd[0] = 0b10100000;
  writeCmd[1] = 0xFF;
  int g = SPI_read(writeCmd);
  Serial.println(g);

}


void ModoWRT() {
  unsigned char writeCmd[num_register] = {0, 0};

  writeCmd[0] = 0b10010000;
  writeCmd[1] = 0;

  Serial.println("Modo WRT");
  //print_cmd(writeCmd);

  SPI_send(writeCmd);
}

void SPI_send(unsigned char* comando) {

  unsigned char writeCmd[num_register] = {0, 0};
  writeCmd[0] = comando[0];
  writeCmd[1] = comando[1];

  print_cmd(writeCmd);

  digitalWrite(chipselect, LOW); // enable shifting
  spi4teensy3::send(writeCmd, num_register);
  digitalWrite(chipselect, HIGH); // disable clock again*/
}

void print_cmd(unsigned char* comando) {

  unsigned char writeCmd[num_register] = {0, 0};
  writeCmd[0] = comando[0];
  writeCmd[1] = comando[1];

  Serial.println("*/////////////////////////////////////* Escribiendo");
  byte i;
  for (i = 0; i < num_register; i++) {
    Serial.print("writeCmd[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(writeCmd[i], BIN);
  }
  Serial.println("");
}
