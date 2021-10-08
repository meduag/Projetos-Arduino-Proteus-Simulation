/*
 This Code is Designed by Prasad Pandit
 www.vhdlcodes.com
 Please keep this header if you are using this code for non-commercial use.
 For using it commercially or posting it on your website contact Author on
 Email: prasadp4009@gmail.com
 
 Instructions:
 Add the glcd folder in your libraries folder in Arduino folder
 It should be like Arduino>libraries>glcd
 
 You need to make changes in pin numbers according to your module.
 You can do that by editing in header files in glcd>config folder.
 I have made changes in k1080_mega.h as per my own needs.
 
 Please make sure to connect a 10K or 5K POT at Analog pin2.
 
  The circuit:
  See the inlcuded documentation in glcd/doc directory for how to wire
  up the glcd module. glcd/doc/GLCDref.htm can be viewed in your browser
  by clicking on the file.
 
 */

// include the library header
#include <glcd.h>
#include "config/ks0108_Mega.h"
#include "fonts/allFonts.h"

int sig_v[113];
void setup() {
  // Initialize the GLCD 
  pinMode(27, OUTPUT);
  digitalWrite(27,HIGH);
  delay(10);
  digitalWrite(27,LOW);
  delay(10);
  digitalWrite(27,HIGH);
  delay(100);
  
  GLCD.Init();   // initialise the library, non inverted writes pixels onto a clear screen
  digitalWrite(27,HIGH);
  GLCD.ClearScreen();
  GLCD.SelectFont(System5x7, BLACK);
  GLCD.CursorToXY( 0,2);
  GLCD.print("5V");
  GLCD.CursorToXY(0,46);
  GLCD.print("0V");
  GLCD.DrawVLine(14, 0, 54, BLACK);
  
  GLCD.DrawHLine(0, 54, 127, BLACK);
  GLCD.DrawHLine(11, 53, 3, BLACK);
  GLCD.DrawHLine(11, 26, 3, BLACK);
  GLCD.DrawHLine(11, 0, 3, BLACK);
  GLCD.CursorToXY(0,57);
  GLCD.print("Ti/Div:");
  
  GLCD.DrawVLine(78, 55, 8, BLACK);
  GLCD.CursorToXY(82,57);
  GLCD.print("A:");
 // Serial.begin(9600);

 // Select the font for the default text area
  }

void loop() {
 // int sig = analogRead(0);
  int test_up,test_down; 
  int time_div;
  int min_amp=53, max_amp=0;
   
  for(int i=0;i<113;i++)
{
 analogWrite(7, i+2);
 //sig= analogRead(0);
 //delayMicroseconds(10);
 sig_v[i] = map(analogRead(0), 0, 1023, 0, 53);
 time_div=analogRead(2)*2;
 delayMicroseconds(time_div); 
}
//GLCD.ClearScreen()   ;


GLCD.FillRect( 15, 0, 112, 53, WHITE);
GLCD.DrawVLine(70, 0, 53, BLACK);
GLCD.DrawHLine(14, 26, 113, BLACK);
  for(int i=0;i<113;i++)
  {
  test_up=(53-sig_v[i+1]) - (53-sig_v[i]);
  test_down=(53-sig_v[i]) - (53-sig_v[i+1]);
  if(test_up>0 && i!=112)
  {
    GLCD.DrawLine( i+15, 53-sig_v[i], i+15, 53-sig_v[i+1], BLACK);
  }
  else if(test_down>0 && i!=112)
  GLCD.DrawLine( i+15, 53-sig_v[i+1], i+15, 53-sig_v[i], BLACK);
  else
   GLCD.SetDot(i+15, 53-sig_v[i], BLACK);
  //Serial.println(sig_v[i]);
  min_amp=min(min_amp,sig_v[i]);
  max_amp=max(max_amp,sig_v[i]);
  
  }
  //GLCD.FillRect( 64, 57, 63, 7, WHITE);
  GLCD.CursorToXY(40,57);
  GLCD.print(time_div);
  GLCD.print("uS");
  GLCD.CursorToXY(94,57);
  GLCD.print((max_amp-min_amp)/10.6);
  GLCD.print("V");
  }
