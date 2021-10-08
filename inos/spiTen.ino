/*
 
  MCP4922 DAC test with spi4teensy3 fast SPI library, Teensy 3.0
 
  mostly pinched from http://forum.pjrc.com/threads/24082-Teensy-3-mcp4921-SPI-Dac-anybody-tried
 
  will need the spi4teensy3 library (https://github.com/xxxajk/spi4teensy3) copying into the libraries folder
 
  at SPI_CLOCK_DIV2 (24MHz on standard Teensy 3.0)
    generates ramp wave with period of 13.8ms (72hz)
    clock period ~42ns
    also faster than the rated speed of the MCP4922 (20Mhz), but it seems to work.
    is updating the voltage out every 3.3us
    
  Teensy pin 10   - MCP pin 3 (SS - slave select)
  Teensy pin 11   - MCP pin 5 (MOSI)
  Teensy pin 13   - MCP pin 4 (SCK - clock)
  +3.3v           - MCP pin 1 (vdd), 13 (DACa vref)
  GND             - MCP pin 8 (LDAC), 12 (Analogue ground ref)
 
 */
 
#include <spi4teensy3.h>  // include the SPI library:
 
const int slaveSelectPin = 10;
int i = 0;
 
void setup() {
  // set the slaveSelectPin as an output:
  pinMode (slaveSelectPin, OUTPUT);
  // initialize SPI:
  spi4teensy3::init();
}
 
void loop() {
  i++;
  write_value(i);
  if(i > 4095) {
    i = 0; 
  }
}
 
void write_value(int value) {
  // channel (0 = DACA, 1 = DACB) // Vref input buffer (0 = unbuffered, 1 = buffered) // gain (1 = 1x, 0 = 2x)  // Output power down power down (0 = output buffer disabled) //  12 bits of data        
  uint16_t out = (0 << 15) | (1 << 14) | (1<< 13) | (1 << 12) | ( value ); 
  digitalWriteFast(slaveSelectPin,LOW);
  spi4teensy3::send(out >> 8);                   //you can only put out one byte at a time so this is splitting the 16 bit value. 
  spi4teensy3::send(out & 0xFF);
  digitalWriteFast(slaveSelectPin,HIGH);   
}
