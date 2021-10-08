
#include <LiquidCrystal.h>
//#include <SoftwareSerial.h>

//#define RxD 0 // Conectar con Tx del arduino
//#define TxD 1 // Conectar con Rx del arduino

LiquidCrystal lcd(13, 14, 15, 16, 17, 18);
const int ledPin = 23;
//char val;
String command = "";
//String st = "";

//SoftwareSerial BTSerial(RxD, TxD);

void setup()
{
  lcd.begin(16, 2);
  //Serial.begin(9600); // USB is always 12 Mbit/sec
  pinMode(ledPin, OUTPUT);
  
  //BTSerial.flush();
  delay(500);
  //BTSerial.begin(9600);
  
  lcd.setCursor(0, 0);
  lcd.print("Teste ");
  delay(1000);
  //Serial.println("Enter AT commands:");
  //delay(1000);
}

void loop()
{

// Read device output if available.
/*   if (BTSerial.available()) {
     while(BTSerial.available()) { // While there is more to be read, keep reading.
       command += (char)BTSerial.read();
       lcd.clear();
     }
     
     Serial.println(command);
     lcd.setCursor(0, 0);
     lcd.print(command);
     
     if(command=="OK"){
       digitalWrite(ledPin, HIGH);   // set the LED on
       delay(1000);                  // wait for a second
       digitalWrite(ledPin, LOW);    // set the LED off
       delay(1000);
       command = ""; // No repeats
     }

   }
   // Read user input if available.
   if (Serial.available()){
       delay(10); // The DELAY!
       BTSerial.write(Serial.read());
   }
  */
}

