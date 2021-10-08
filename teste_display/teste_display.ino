#include <Wire.h>

#include <SPI.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int i = 0;

// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 10;


void setup() {
  Serial.begin(9600);

  // set the slaveSelectPin as an output:
  pinMode (slaveSelectPin, OUTPUT);
  digitalWrite (slaveSelectPin, HIGH);
  SPI.begin();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  delay(2000);
}

void loop() {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  //display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  // display.write(1);
  display.print(F("scroll: "));
  display.println(i);
  i += 1;
  display.display();
  delay(100);
}



void digitalPotWrite(int address, int value) {
  //SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
  // take the SS pin low to select the chip:
  digitalWrite(slaveSelectPin,LOW);
  //  send in the address and value via SPI:
  SPI.transfer(address);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin,HIGH);
  // release control of the SPI port
  SPI.endTransaction();
}
