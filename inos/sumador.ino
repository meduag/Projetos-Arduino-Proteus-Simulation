int vCh1 = 0, vCh2 = 0, vCh3 = 0, vCh4 = 0;
byte op = 0b00000000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  vCh1 = 1, vCh2 = 0, vCh3 = 0, vCh4 = 0;
  op = (vCh4 << 4) | (vCh3 << 3) | (vCh2 << 2) | vCh1;
  Serial.print("Valor de la suma en binario: ");
  Serial.println(op, BIN);
  Serial.print("Valor de la suma en decimal: ");
  Serial.println(op, DEC);
  delay(2000);

  Serial.println(" ");
  vCh1 = 1, vCh2 = 1, vCh3 = 0, vCh4 = 0;
  op = (vCh4 << 3) | (vCh3 << 2) | (vCh2 << 1) | vCh1;
  Serial.print("Valor de la suma en binario: ");
  Serial.println(op, BIN);
  Serial.print("Valor de la suma en decimal: ");
  Serial.println(op, DEC);
  delay(2000);

  Serial.println(" ");;

  vCh1 = 1, vCh2 = 1, vCh3 = 1, vCh4 = 0;
  op = (vCh4 << 3) | (vCh3 << 2) | (vCh2 << 1) | vCh1;
  Serial.print("Valor de la suma en binario: ");
  Serial.println(op, BIN);
  Serial.print("Valor de la suma en decimal: ");
  Serial.println(op, DEC);
  delay(2000);

  Serial.println(" ");

  vCh1 = 1, vCh2 = 1, vCh3 = 1, vCh4 = 1;
  op = (vCh4 << 3) | (vCh3 << 2) | (vCh2 << 1) | vCh1;
  Serial.print("Valor de la suma en binario: ");
  Serial.println(op, BIN);
  Serial.print("Valor de la suma en decimal: ");
  Serial.println(op, DEC);
  delay(2000);

  Serial.println(" ");

  vCh1 = 0, vCh2 = 1, vCh3 = 1, vCh4 = 0;
  op = (vCh4 << 3) | (vCh3 << 2) | (vCh2 << 1) | vCh1;
  Serial.print("Valor de la suma en binario: ");
  Serial.println(op, BIN);
  Serial.print("Valor de la suma en decimal: ");
  Serial.println(op, DEC);
  delay(2000);

  Serial.println(" ");


}
