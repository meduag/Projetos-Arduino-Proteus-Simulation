#include <Wire.h>
#include <ADXL345.h>

ADXL345 adxl;
int x, y, z;
char output[512];

void setup() {
  adxl.powerOn();
  Serial.begin(9600);
}

void loop() {
  adxl.readAccel(&x, &y, &z);

  sprintf(output, "X= %d  ; Y= %d  ; Z= %d", x, y, z);
  Serial.print(output);
  Serial.println();

  x = map(x, -255, 255, 179, 0);
  y = map(y, -255, 255, 0, 179);

  sprintf(output, "%d ; %d ", x, y);
  Serial.print(output);
  Serial.println();

  delay(1000);
}

