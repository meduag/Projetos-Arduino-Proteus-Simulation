#include <MEGP_DAC124S085.h>

void setup() {
  // put your setup code here, to run once:
  MEGP_DAC124S085(13);
}

void loop() {
  Send_Dac(0, 1, 4095);
  delay(1000);
  Send_Dac(0, 1, 0);
  delay(1000);


}
