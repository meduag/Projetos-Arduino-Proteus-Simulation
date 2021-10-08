/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsESP8266.h"

/****************************************
 * Define Constants
 ****************************************/
namespace {
  const char * WIFISSID = "Baeza sala"; // Assign your WiFi SSID
  const char * PASSWORD = "Baeza sala"; // Assign your WiFi password
  const char * TOKEN = "BBFF-THEyBBLGfTxR4xaGrar3EraxqDNO6A"; // Assign your Ubidots TOKEN
}

Ubidots client(TOKEN);

/****************************************
 * Main Functions
 ****************************************/
void setup() {
  Serial.begin(115200);
  client.wifiConnection(WIFISSID, PASSWORD);
}

void loop() {
  client.readData(); // Reads the command from the logger
  delay(1000);
}
