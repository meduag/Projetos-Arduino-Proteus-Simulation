#include <AS5040.h>

// CHANGE THESE AS APPROPRIATE
#define CSpin   10
#define CLKpin  13
#define DOpin   12

// OUTPUT PINS
#define enc_a  5
#define enc_b  6
#define enc_z  7

int reEnc = 0;
int reEnc2 = 0;
int amostras = 10;
int c = 0;
int ptr = 1;


AS5040 enc (CLKpin, CSpin, DOpin) ;

// Set mode to quadrature (A + B + index), monitor via serial IF
void setup ()
{
  Serial.begin (115200) ;   // NOTE BAUD RATE
  if (!enc.begin ())
    Serial.println ("Error setting up AS5040") ;
}

void loop ()
{
  for (int i = 0; i <= amostras; i++)  {
    reEnc = reEnc + enc.read ();
  }

  reEnc = reEnc / amostras;

  if (c <= 10) {
    reEnc2 = reEnc2 + reEnc;
    c = c + 1;
  }

  if (c == 11) {
    reEnc2 = reEnc2 / 10;
    if (ptr == 0 ) {
      Serial.println(reEnc2) ;
    } else {
      Serial.print(reEnc2) ;
      Serial.print ("   ") ;
      Serial.print (enc.status (), BIN) ;
      Serial.print ("   ") ;
      Serial.print (enc.valid () ? "OK" : "Fault") ;
      Serial.print ("   ") ;
      Serial.println (enc.Zaxis ()) ;
    }
    c = 0;
    reEnc2 = 0;
  }

  reEnc = 0;

  //Serial.println(enc.read ());
  //delay(100);

  /*Serial.print ("   ") ;
    Serial.print (enc.status (), BIN) ;
    Serial.print ("   ") ;
    Serial.print (enc.valid () ? "OK" : "Fault") ;
    Serial.print ("   ") ;
    Serial.println (enc.Zaxis ()) ;
    delay (100) ;*/
}
