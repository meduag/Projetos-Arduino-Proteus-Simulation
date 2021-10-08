#include <SoftwareSerial.h>

SoftwareSerial BTSerial(8,9);

int comm_status;

void setup()
{
  comm_status = 0;
  
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop()
{
  if (BTSerial.available() > 0)
  {
    String str = BTSerial.readString();

    Serial.println("Android:");
    for (int i = 0; i < str.length(); i++)
    {
      if (i != 0)
        Serial.print(", ");  
      Serial.print(str[i], HEX);
    }

    delay(1000);
    byte ans[6] = {0x24, 0x20, 0x01, 0x00, 0x21, 0x3B};
    sendData(ans, 6);
    comm_status = 1;
    delay(1000);
  }

  switch (comm_status)
  {      
    case 1:
    {
      byte ans[13] = {0x25, 0x10, 0x08, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x04, 0x24, 0x3B};
      sendData(ans, 13);
      comm_status = 2;
      delay(1000);
      break;
    }
      
    case 2:
    {
      byte ans[13] = {0x25, 0x10, 0x08, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x04, 0x24, 0x3B};
      sendData(ans, 13);
      comm_status = 3;
      delay(1000);
      break;
    }

    case 3: 
    {
      byte ans[13] = {0x25, 0x10, 0x08, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x04, 0x24, 0x3B};
      sendData(ans, 13);
      comm_status = 4;
      delay(1000);
      break;
    }

    case 4:
    {
      byte ans[7] = {0x25, 0x10, 0x02, 0x01, 0x01, 0x14, 0x3B};
      sendData(ans, 7);
      comm_status = 5;
      delay(1000);
      break;
    }
  }
}

void sendData(byte* out, int len)
{
  Serial.println("\nHC05:");
  
  for (int i = 0; i < len; i++)
  {
    BTSerial.write(out[i]);
    
    if (i != 0)
      Serial.print(", ");
    Serial.print(out[i], HEX);
  }
}

