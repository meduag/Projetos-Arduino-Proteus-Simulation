const int redPin =  2;
const int greenPin = 3;
const int bluePin =  4;

void setup()   {                
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop()                     
{
  analogWrite(redPin, 100);
  analogWrite(greenPin, 200);
  //analogWrite(bluePin, 250);
}
