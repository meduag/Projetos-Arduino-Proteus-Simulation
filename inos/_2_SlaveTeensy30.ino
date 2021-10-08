#include <Wire.h>
const int ledPin = 13;

void setup()
{
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);    // set the LED off
  Wire.begin(2); // Abrimos el canal 2 (0x02) del I2C
  Wire.onRequest(peticion);// Creamos el evento que se relaizará cuando el Receptor llame a el emisor (Este Arduino)
  Serial.begin(9600);
}

void loop()
{
}

void peticion() 
{
  digitalWrite(ledPin, HIGH);   // set the LED on
  Serial.print("\n Se pidio el mensaje, Enviando desde canal 2");//Imprimios cuando el receptor  nos pide el mensaje
  Wire.write("Enviando canal 2");//Imprimimos cuando se realize la peticion ese mensaje y lo enviaremos por el canal 1
  digitalWrite(ledPin, LOW);    // set the LED off
}
