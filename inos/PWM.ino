const int PWM_Pin =  3;

void setup() {
  // put your setup code here, to run once:
  pinMode(PWM_Pin, OUTPUT);
  analogWriteFrequency(PWM_Pin, 750000); // Teensy 3.0 pin 3 also changes to 375 kHz
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(PWM_Pin, 4);
}
