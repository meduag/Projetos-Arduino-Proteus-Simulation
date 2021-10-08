/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  This example code is in the public domain.
*/

// Pin 13 has an LED connected on most Arduino boards.
// Pin 11 has the LED on Teensy 2.0
// Pin 6  has the LED on Teensy++ 2.0
// Pin 13 has the LED on Teensy 3.0
// give it a name:
int led = 13;

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(0, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(2000000);
}

  // the loop routine runs over and over again forever:
  void loop() {
    int a = analogRead(A0);
    Serial.println(a);
    
    /*digitalWrite(0, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);               // wait for a second
    Serial.println("Led OFF");
    digitalWrite(0, LOW);    // turn the LED off by making the voltage LOW
    delay(100);               // wait for a second
    Serial.println("Led ON");*/
  }
