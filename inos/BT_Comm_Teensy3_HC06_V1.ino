String com = "";

int c = 0;

void setup()
{
  //Inicia a serial
  Serial1.begin(9600);
  Serial.begin(9600);
  Serial.println("Digite os comandos AT :");
}

void loop()
{
  com = ""; // limpiio la variable a cada vez
  if (Serial1.available())
  {
    while (Serial1.available()) {
      com += (char)Serial1.read();
      delay(10);
    }
    Serial.print("Dato: ");
    Serial.print(com);
    com = "";

    Serial1.println("{");//inicio del Mensage
    
    if (com.indexOf("lED1")) {
      c += 1;
      Serial1.print("Contador: ");
      Serial1.println(c);
      Serial.print(" Contador: ");
      Serial.println(c);
    }

    Serial1.println("}");//fin del mensaje

  }
}


