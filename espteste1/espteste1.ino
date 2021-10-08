/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsESPMQTT.h"

/****************************************
 * Define Constants
 ****************************************/
#define TOKEN "BBFF-THEyBBLGfTxR4xaGrar3EraxqDNO6A" // Your Ubidots TOKEN
#define WIFINAME "Baeza sala" //Your SSID
#define WIFIPASS "andrea76" // Your Wifi Pass
#define DEVICE_LABEL  "my-new-device"  // Put here your Ubidots device label
#define VARIABLE_LABEL  "led1"  // Put here your Ubidots variable label 

Ubidots client(TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  // put your setup code here, to run once:
  client.ubidotsSetBroker("business.api.ubidots.com"); // Sets the broker properly for the business account
  client.setDebug(false); // Pass a true or false bool value to activate debug messages
  Serial.begin(115200);
  client.wifiConnection(WIFINAME, WIFIPASS);
  client.begin(callback);
  client.ubidotsSubscribe(DEVICE_LABEL, VARIABLE_LABEL); //Insert the dataSource and Variable's Labels
  }

void loop() {
  // put your main code here, to run repeatedly:
  if(!client.connected()){
      client.reconnect();
      client.ubidotsSubscribe(DEVICE_LABEL, VARIABLE_LABEL); //Insert the dataSource and Variable's Labels
      }
  client.loop();
  }
