String datosIn = "";

void setup()   {                
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  delay(1000);
  digitalWrite(13,HIGH);
  delay(2000);
  digitalWrite(13,LOW);
 
}

void loop()                     
{
  if(Serial.available()>0){
    datosIn = Serial.read();
    digitalWrite(13,HIGH);
  }
  //Serial.println(datosIn);

}
