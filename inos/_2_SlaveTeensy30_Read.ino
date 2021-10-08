#include <Wire.h>

const int ledPin = 13;
char mensaje= 'n';//INICIAMOS UNA CADENA

void setup(){
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);    // set the LED off
  Wire.begin(2); // Abrimos el canal 2 (0x02) del I2C
  //Wire.onRequest(peticion);// Creamos el evento que se relaizará cuando el Receptor llame a el emisor (Este Arduino)
  Serial.begin(9600);
}

void loop(){
  while (Wire.available())
  {
    mensaje = Wire.read();//GUARDAMOS EL MENSAJE EN LA CADENA MESAJE 1
    Serial.print(mensaje);//IMPRIMIMOS EL MENSAJE
    delay(50);
  }
  Serial.println(mensaje);
}


