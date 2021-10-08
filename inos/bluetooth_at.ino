/* *
 * Code based on Instructables tutorial: http://www.instructables.com/id/Modify-The-HC-05-Bluetooth-Module-Defaults-Using-A/?ALLSTEPS
 * For the HC05 board used, the pushbutton was be pressed while powering it on.
 * And on the reference sheet: http://www.instructables.com/files/orig/FOR/4FP2/HKZAVRT6/FOR4FP2HKZAVRT6.pdf 
 * 
 * This script enables communication with the HC05 board while in AT mode
 * using Arduino IDE's serial monitor.
 *  */
char received_byte[20];

void setup()
{
  // Changing HC05 to AT mode (if button is not present in board)
//  pinMode(7, OUTPUT);
//  digitalWrite(7, HIGH);
  
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(1000);
  Serial.println("Enter AT commands:");
}

void loop()
{
  
  if (Serial1.available())
  {
    received_byte[20] = Serial1.read();
//    Serial.print(received_byte, HEX);
    Serial.print(received_byte);
  }
  if (Serial.available())
  {
    received_byte[20] = Serial.read();
//    Serial.print(received_byte, HEX);
    Serial.print(received_byte);
    Serial1.write(received_byte);
  }
}
