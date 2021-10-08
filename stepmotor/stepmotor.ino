int Index;
int t = 800;
void setup()
{
  pinMode(6, OUTPUT); //Enable
  pinMode(5, OUTPUT); //Step
  pinMode(4, OUTPUT); //Direction

  digitalWrite(6, LOW);
}

void loop()
{
  digitalWrite(4, HIGH);

  for (Index = 0; Index < 2000; Index++)
  {
    digitalWrite(5, HIGH);
    delayMicroseconds(t);
    digitalWrite(5, LOW);
    delayMicroseconds(t);
  }

  digitalWrite(4, LOW);

  digitalWrite(6, HIGH);
  delay(3000);
  digitalWrite(6, LOW);

  for (Index = 0; Index < 2000; Index++)
  {
    digitalWrite(5, HIGH);
    delayMicroseconds(t);
    digitalWrite(5, LOW);
    delayMicroseconds(t);
  }

  digitalWrite(6, HIGH);
  delay(3000);
  digitalWrite(6, LOW);
}
