  
/*  
GraphicsTest.pde  
>>> Before compiling: Please remove comment from the constructor of the   
>>> connected graphics display (see below).  
  
Universal 8bit Graphics Library, http://code.google.com/p/u8glib/  
  
Copyright (c) 2012, olikraus@gmail.com  
All rights reserved.  
 
Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions 
are met:  
 
* Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.  
     
* Redistributions in binary form must reproduce the above copyright 
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.  
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND   
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,   
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF   
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE   
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR   
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,   
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT   
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;   
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER   
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,   
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)   
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF   
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.   
*/  
   
#include "U8glib.h"  
// A linha abaixo define as ligacoes e deve ser 
// ajustada conforme o display utilizado. 
U8GLIB_ST7920_128X64_1X u8g(6, 5, 4 ,7); //Enable, RW, RS, RESET  
int display = 1;  
   
void u8g_prepare() 
{  
  u8g.setFont(u8g_font_6x10);  
  u8g.setFontRefHeightExtendedText();  
  u8g.setDefaultForegroundColor();  
  u8g.setFontPosTop();  
}  
   
void u8g_Tela1()  //Tela 1 - Arduino e Cia - Retangulos  
{
  u8g.setFont(u8g_font_unifont);  
  u8g.drawStr( 11, 35, "Arduino e Cia");  
  u8g.drawStr( 12, 35, "Arduino e Cia");  
  u8g.drawFrame(0,0,128,64);  
  u8g.drawFrame(2,2,124,60);   
}  
   
void u8g_Tela2() //Tela 2 - Moldura e relógio  
{
  u8g.drawRFrame(0,0,128,64,3);  
  u8g.drawStr( 3, 10, "Hor.: 13:00");  
  u8g.drawStr( 3, 25, "Temp: 27");  
  char s[2] = " ";  
  s[0] = 176;  
  u8g.drawStr(51, 25, s);  
  u8g.drawStr( 3, 40, "Umid: 25%");  
  u8g.drawCircle(95,32,28);  
  u8g.drawCircle(95,32,29);  
  u8g.drawLine(95, 9, 95, 4);  
  u8g.drawLine(123, 32, 118, 32);  
  u8g.drawLine(95, 55, 95, 60);  
  u8g.drawLine(67, 32, 72, 32);  
  u8g.drawLine(95, 32, 95, 12);  
  u8g.drawLine(95, 32, 100.8, 21.87 );  
  u8g.setFont(u8g_font_04b_03);  
  u8g.drawStr(89,43, "Tag");  
  u8g.drawStr(85,50, "Heuer");  
}  
   
void u8g_Tela3() //Tela 3 - Caracteres Ascii - Pag. 1  
{
  char s[2] = " ";  
  u8g.drawStr( 0, 0, "ASCII page 1");  
  for(int y = 0; y < 6; y++ ) 
  {  
    for(int x = 0; x < 16; x++ ) 
    {
      s[0] = y*16 + x + 32;  
      u8g.drawStr(x*7, y*10+10, s);  
    }  
  }  
}  
   
void u8g_Tela4()  //Tela 3 - Caracteres Ascii - Pag. 2  
{
  char s[2] = " ";  
  uint8_t x, y;  
  u8g.drawStr( 0, 0, "ASCII page 2");  
  for( y = 0; y < 6; y++ ) 
  {  
    for( x = 0; x < 16; x++ ) 
    {  
      s[0] = y*16 + x + 160;  
      u8g.drawStr(x*7, y*10+10, s);  
    }  
  }  
}  
   
void u8g_Tela5() //Tela 5 - Arduino e Cia - Retangulo preenchido  
{
  u8g.setFont(u8g_font_unifont);  
  u8g.drawBox(0,0,128,64);  
  u8g.drawBox(2,2,124,60);   
  u8g.setColorIndex(0);  
  u8g.drawStr( 11, 35, "Arduino e Cia");  
  u8g.drawStr( 12, 35, "Arduino e Cia");  
  u8g.drawFrame(2,2,124,60);  
}   
   
void u8g_Tela6()  //Tela 6 - Arduino e Cia em 0, 90 e 270 graus  
{
  u8g.setFont(u8g_font_helvB08);  
  u8g.drawStr(50,31, " Arduino");  
  u8g.drawStr90(50,31, " e");  
  u8g.drawStr270(50,31, " Cia");  
}  
   
void u8g_Tela7() //Tela 7 - Fontes diferentes  
{
  u8g.setFont(u8g_font_robot_de_niro);  
  u8g.drawStr(5,13, "Arduino e Cia !");  
  u8g.setFont(u8g_font_helvB08);  
  u8g.drawStr(5,25, "Arduino e Cia !");  
  u8g.drawBox(5,31, 118,11);  
  u8g.setColorIndex(0);  
  u8g.setFont(u8g_font_8x13);  
  u8g.drawStr(5,41, "Arduino e Cia !");  
  u8g.setColorIndex(1);  
  u8g.setFont(u8g_font_ncenB10);  
  u8g.drawStr(5,60, "Arduino e Cia !");  
}  
   
void draw() //Rotina Desenho  
{
  u8g_prepare();  
  switch(display) //Carrega a tela correspondente  
  {
   case 1:  
    u8g_Tela1();  
    break;  
   case 2:  
    u8g_Tela2();  
    break;  
   case 3:  
    u8g_Tela3();  
    break;  
   case 4:  
    u8g_Tela4();  
    break;  
   case 5:  
    u8g_Tela5();  
    break;  
   case 6:  
    u8g_Tela6();  
    break;  
   case 7:  
    u8g_Tela7();  
    break;  
  }  
}  
   
void setup() 
{  
  // flip screen, if required  
  //u8g.setRot180();  
   
  // assign default color value  
  if ( u8g.getMode() == U8G_MODE_R3G3B2 )   
   u8g.setColorIndex(255);   // white  
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT )  
   u8g.setColorIndex(1);     // max intensity  
  else if ( u8g.getMode() == U8G_MODE_BW )  
   u8g.setColorIndex(1);     // pixel on  
    
  //u8g.setContrast(0x30);  
}  
   
void loop() 
{  
  // picture loop   
  for (display =1; display < 8; display++) //Carrega as telas de 1 a 7  
  {  
    u8g.firstPage();   
    do 
    {  
    draw();  
    } 
    while( u8g.nextPage() );  
    delay(3000);  //Pausa de 3 segundos e reinicia o processo  
  }  
}     
