#include <ESP8266WiFi.h>

#define WIFI_SSID "Abuelita Borracha 2.4G"
#define WIFI_PASS "vaitomarnocu123"

//#define WIFI_SSID "Meduag"
//#define WIFI_PASS "asd123456"

void setup() {
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.print("Connecting");
  
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  // put your main code here, to run repeatedly:

}
