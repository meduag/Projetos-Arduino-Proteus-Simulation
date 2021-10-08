#include <Wire.h>

char mensaje = 'n'; //INICIAMOS UNA CADENA

void setup() {
  Wire.begin(2); // Abrimos el canal 2 (0x02) del I2C
  Serial.begin(9600);
  Wire.onReceive(recibidoEvento);   // Al activarse el evento de lectura, se ejecuta la funcion recibidoEvento
}

void loop() {
  if (mensaje != 'n' ) {
    Serial.println(mensaje);
    mensaje = 'n';
  }

  delay(100);
}

void recibidoEvento(int recepcion)  // Evento de recepción
{
  //unsigned int pedido;
  while ( Wire.available())            // Leemos hasta que no haya datos, osea el 1 que va a enviar el maestro
  {
    mensaje = Wire.read();                  // Leemos el 1
    Serial.print(mensaje);               // Imprimimos el 1 por serial para testear
  }
}
