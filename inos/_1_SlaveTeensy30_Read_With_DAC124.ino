#include <Wire.h>
#include <SPI.h>

char mensaje = 'n'; //INICIAMOS UNA CADENA
int ss = 10;

void setup() {
  pinMode(ss, OUTPUT);
  Wire.begin(2); // Abrimos el canal 2 (0x02) del I2C
  SPI.begin();
  Serial.begin(9600);
  Serial.println("Esperando envio de datos, DAC 2048");//IMPRIMIMOS EL MENSAJE

  enviarValor(0, 1, 0);//mitad del dac osea 0 vcd
  digitalWrite(ss, HIGH);
  Wire.onReceive(RecibirDado);   // Al activarse el evento de lectura, se ejecuta la funcion recibidoEvento

}

void loop() {
  int comunication = 0;

  if (mensaje != 'n' ) {
    if (comunication == 0) {
      enviarValor(0, 1, 2048);//mitad del dac osea 0 vcd
      digitalWrite(ss, HIGH);
      comunication = 1;
    }

    Serial.println(mensaje);
    mensaje = 'n';
  }

  /*
   * ">" indicava o inicio duma mensagem  
   * "$" indicava o fim da mensagem  
   * ":" eram valores delimitadores. Se quiseres podes usar outros caracteres e não precisam de ser sempre iguais.  
  */


  /*enviarValor(0, 1, 4095);
  delay(1000);
  enviarValor(0, 1, 2048);
  delay(1000);
  enviarValor(0, 1, 0);*/

  //delay(100);
}

void enviarValor(int DacABCD, int MoP, uint16_t value) {
  byte valueToWriteH = 0;
  byte valueToWriteL = 0;

  valueToWriteH = highByte(value);
  valueToWriteH = 0b00001111 & valueToWriteH;
  valueToWriteH = (DacABCD << 6 ) | (MoP << 4) | valueToWriteH;

  valueToWriteL = lowByte(value);

  // take the SS pin low to select the chip:
  digitalWrite(ss, LOW);
  //  send in the address and value via SPI:
  SPI.transfer(valueToWriteH);
  SPI.transfer(valueToWriteL);
  // take the SS pin high to de-select the chip:
  digitalWrite(ss, HIGH);
}

void RecibirDado(int recepcion)  // Evento de recepción
{
  //unsigned int pedido;
  while ( Wire.available())            // Leemos hasta que no haya datos, osea el 1 que va a enviar el maestro
  {
    mensaje = Wire.read();                  // Leemos el 1
    Serial.print(mensaje);               // Imprimimos el 1 por serial para testear
  }
}

