String com = "";

byte *in_message;

void setup()
{
  //Inicia a serial
  Serial1.begin(9600);
  Serial.begin(9600);
  Serial.println("Digite os comandos AT :");
  //Inicia a serial configurada nas portas 6 e 7

}

void loop()
{
  if (Serial1.available())
  {
    /*  while(Serial1.available())
      {
        com += (char)Serial1.read();
      }
      Serial.println(com);
      com = "";
      }

      if (Serial.available())
      {
      delay(10);
      Serial1.write(Serial.read());*/


    in_message = NULL;
    
    Serial.println("Before reading...");
    in_message = readBTMessage(1);

    Serial.println("After reading...");
    
    for (int iterator = 0; iterator < 10; iterator++) {
      Serial.print(in_message[iterator], DEC);
      Serial.print(" ");
    }
    Serial.println();

    Serial.println("START");


  }
}


byte* readBTMessage(int size2)
{
  byte* in_bytes = NULL;
  in_bytes = (byte*) malloc(size2 * sizeof(byte));
  in_bytes[0] = 0;

  if (Serial1.available() > 0)
    Serial1.readBytes(in_bytes, size2);

  Serial.println(in_bytes[0], DEC);
  return in_bytes;
}

