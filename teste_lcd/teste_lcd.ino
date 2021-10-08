/* Include the U8glib library */
#include "U8glib.h"

/* Define the SPI Chip Select pin */
#define CS_PIN 10

/* Create an instance of the library for the 12864 LCD in SPI mode */
U8GLIB_ST7920_128X64_1X u8g(CS_PIN);

void setup() 
{
}

/* Main program */
void loop() 
{
  /* Start of a picture loop */
  u8g.firstPage();  
  
  /* Keep looping until finished drawing screen */
  do 
  {
    /* Set the font */
    u8g.setFont(u8g_font_courB14);
    
    /* Display some text */
    u8g.drawStr( 35, 26, "HOBBY");
    u8g.drawStr( 8, 46, "COMPONENTS");
    
    /* Draw a simple border */
    u8g.drawFrame(5,5,117,54);
    u8g.drawFrame(3,3,121,58);
  
  }while(u8g.nextPage());
}
