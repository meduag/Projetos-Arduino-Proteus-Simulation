#include <ESP8266WiFi.h>

void setup()
{
  Serial.begin(115200);
  Serial.println();
  delay(100);

  WiFi.begin("Baeza sala", "lasneaux70");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {}
