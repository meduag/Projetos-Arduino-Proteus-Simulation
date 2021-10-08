#include <SoftwareSerial.h>

int bluetoothTx = 0;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 1;  // RX-I pin of bluetooth mate, Arduino D3

int led = 13;

byte in ;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup()
{
  Serial.begin(9600);  // Begin the serial monitor at 9600bps

  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  bluetooth.begin(9600);  // Start bluetooth serial at 9600
  pinMode(led, OUTPUT);
}

void loop()
{

  if (bluetooth.available()) {

    in = bluetooth.read();
    String inS = String(in);
    Serial.println(inS);

    //    //Serial.println(dataFromBt);
    //    if (dataFromBt == '1') {
    //      Serial.println("led on");
    //      digitalWrite(led, HIGH);
    //      bluetooth.print("1");
    //    }
    //    if (dataFromBt == '0') {
    //      Serial.println("led off");
    //      digitalWrite(led, LOW);
    //      bluetooth.print("0");
    //    }
    //    if (dataFromBt == 'b') {
    //      Serial.println("a");
    //      lightBlink = true;
    //    } else {
    //      lightBlink = false;
    //    }

  }

  //  if (Serial.available()) // If stuff was typed in the serial monitor
  //  {
  //    int bytes=Serial.available();
  //    //Serial.readBytes(buffer, startPosition, bytes);
  //
  //    bluetooth.print((char)Serial.read());
  //
  //  }


}
