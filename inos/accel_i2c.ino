#include "IMU.h"

IMUClass IMU;

void setup()
{
  IMU.begin(0);
}

void loop()
{
  IMU.read();
  IMU.show(0);

  delay(100);
}