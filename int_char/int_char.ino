int a = 1;
char b[2];
String str;

void setup() {
  str = String(a);
  str.toCharArray(b, 2);
  Serial.begin(115200);
  delay(1000);
  Serial.println(str);
}

void loop() {
  // put your main code here, to run repeatedly:

}
