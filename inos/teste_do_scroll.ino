#include "stdlib.h"
#include "math.h"
#include "SPI.h" 
#include <LiquidCrystal.h>

int del=0; 
int ss= 53;
int OsVal;
float sinVal;
int a =3098;
word outputValue = 0; // 16-bit 
byte data = 0; // 8-bit 

float Vmed;
float Imed;
float V = 0; // valores que serão usados para começar a interface, 10 volts, 30 ohms e 30miliAmperes
float R = 0;
float I = 0;



LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7); // define os pinos utilizados


int lcd_key     = 0;       // define os valores representativos de cada botão
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5


int read_LCD_buttons()   // função para ler os valores
{
 adc_key_in = analogRead(0);      
 if (adc_key_in > 1000) return btnNONE; 
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  
  
  return btnNONE;  // when all others fail, return this...
}



         
                  
 
          


        void Onda_Quadrada(float Tensao, float Res, float Corrente){
  
                 outputValue = 4095;
                  digitalWrite(ss, LOW);
                  data = highByte(outputValue);
                  data = 0b00001111 & data;
                  data = 0b00110000 | data;
                  SPI.transfer(data);
                  data = lowByte(outputValue);
                  SPI.transfer(data);
                  digitalWrite(ss, HIGH);
                  delay(1000);
                  //delayMicroseconds(1);
                  //delay250ns();
                //}
                //delay(del+25);
  
                  outputValue = 0;
                  digitalWrite(ss, LOW);
                  data = highByte(outputValue);
                  data = 0b00001111 & data;
                  data = 0b00110000 | data;
                  SPI.transfer(data);
                  data = lowByte(outputValue);
                  SPI.transfer(data);
                  digitalWrite(ss, HIGH);
                  delay(1000);
                  }
                  
     /* for ( a=4095; a>=0; --a)
  {
    outputValue = a;
    digitalWrite(ss, LOW);
    data = highByte(outputValue);
    data = 0b00001111 & data;
    data = 0b00110000 | data;
    SPI.transfer(data);
    data = lowByte(outputValue);
    SPI.transfer(data);
    digitalWrite(ss, HIGH);
    delayMicroseconds(1);
    //delay250ns();
  } */
  //delay(del+25);
  
  
        void Onda_Escada (float Tensao){
  
                  outputValue = 4095;
                    digitalWrite(ss, LOW);
                    data = highByte(outputValue);
                    data = 0b00001111 & data;
                    data = 0b00110000 | data;
                    SPI.transfer(data);
                    data = lowByte(outputValue);
                    SPI.transfer(data);
                    digitalWrite(ss, HIGH);
                    delay(1000);
                    //delayMicroseconds(1);
                    //delay250ns();
                  //}
                  //delay(del+25);
                  
                    outputValue = 2048;
                    digitalWrite(ss, LOW);
                    data = highByte(outputValue);
                    data = 0b00001111 & data;
                    data = 0b00110000 | data;
                    SPI.transfer(data);
                    data = lowByte(outputValue);
                    SPI.transfer(data);
                    digitalWrite(ss, HIGH);
                    delay(1000);
                    //delayMicroseconds(1);
                      
                    outputValue = 0;
                    digitalWrite(ss, LOW);
                    data = highByte(outputValue);
                    data = 0b00001111 & data;
                    data = 0b00110000 | data;
                    SPI.transfer(data);
                    data = lowByte(outputValue);
                    SPI.transfer(data);
                    digitalWrite(ss, HIGH);
                    delay(1000);
                    //delayMicroseconds(1);
                    
                    outputValue = 2048;
                    digitalWrite(ss, LOW);
                    data = highByte(outputValue);
                    data = 0b00001111 & data;
                    data = 0b00110000 | data;
                    SPI.transfer(data);
                    data = lowByte(outputValue);
                    SPI.transfer(data);
                    digitalWrite(ss, HIGH);
                    delay(1000);
                    //delayMicroseconds(1);  
                    
                    }
    
    
          void Onda_Senoidal(float Tensao){

                    for (int x=30; x<=150; x++) {
                      sinVal = (sin(x*(3.1412/180)));
                      OsVal = int(sinVal*4095);
                     outputValue = OsVal;
                      digitalWrite(ss, LOW);
                      data = highByte(outputValue);
                      data = 0b00001111 & data;
                      data = 0b00110000 | data;
                      SPI.transfer(data);
                      data = lowByte(outputValue);
                      SPI.transfer(data);
                      digitalWrite(ss, HIGH);
                    delay(10);
                  }
                  
                   for (int x=330; x>210; x--) {
                      sinVal = (sin(x*(3.1412/180)));
                      OsVal = int(sinVal*4095);
                     outputValue = OsVal;
                      digitalWrite(ss, LOW);
                      data = highByte(outputValue);
                      data = 0b00001111 & data;
                      data = 0b00110000 | data;
                      SPI.transfer(data);
                      data = lowByte(outputValue);
                      SPI.transfer(data);
                      digitalWrite(ss, HIGH);
                    delay(10);
                  }
                    
                  }
  

            void Onda_Exponencial(float Tensao){

                    float base = 2.72;
                    float x;
                    double val;
                    double res;
                    int y;
                    
                    
                     for ( x=0; x<=8.3; x++) {
                       
                        val = pow(2.72,x) ;
                       outputValue = val;
                        digitalWrite(ss, LOW);
                        data = highByte(outputValue);
                        data = 0b00001111 & data;
                        data = 0b00110000 | data;
                        SPI.transfer(data);
                        data = lowByte(outputValue);
                        SPI.transfer(data);
                        digitalWrite(ss, HIGH);
                      delay(1000);
                    }
                    
            }  
                    
                     void Escolhe_Corrente(){
                  
                      bool ok = true;
                      while(ok){
               
                   
                    lcd_key = read_LCD_buttons();  // Lê os botões
                     
                  
                     switch (lcd_key)               // Para cada botão escolhido, uma ação acontece
                     {
                       case btnRIGHT:
                         {
                         lcd.print("RIGHT ");
                         break;
                         }
                       case btnLEFT:
                         {
                         lcd.print("LEFT   ");
                         break;
                         }
                       case btnUP:
                         {
                         //lcd.print("UP    ");  
                         lcd.clear();
                         I++;
                         Imed=(I/200);
                         lcd.print(Imed);
                         lcd.print(" [A]");
                         break;
                         }
                       case btnDOWN:
                         {
                         //lcd.print("DOWN  ");
                         lcd.clear();
                         I--;
                         Imed=(I/200);
                         lcd.print(Imed);
                         lcd.print(" [A]");
                         break;
                         }
                         
                        case btnNONE:
                           {
                         //lcd.print("NONE  ");
                         break;
                         }
                         
                       case btnSELECT:
                       {
                      
                       lcd.clear();  
                       lcd.print("Chosen Current: ");
                       lcd.setCursor(0,1);
                       lcd.print( Imed);
                       lcd.print(" [I]");
                       delay(2000);
                      
                       
                       ok = false;
                       break;
                       
                      
                       }
                       
                     }
                       

                     }
             
                   } 
                    
                    
                    
               void Escolhe_Voltagem(){
                
                bool ok = true;
                while(ok){
               
                   
                    lcd_key = read_LCD_buttons();  // read the buttons
                     
                  
                     switch (lcd_key)               // depending on which button was pushed, we perform an action
                     {
                       case btnRIGHT:
                         {
                         lcd.print("RIGHT ");
                         break;
                         }
                       case btnLEFT:
                         {
                         lcd.print("LEFT   ");
                         break;
                         }
                       case btnUP:
                         {
                         //lcd.print("UP    ");  
                         lcd.clear();
                         V++;
                         Vmed=(V/200);
                         lcd.print(Vmed);
                         lcd.print(" [V]");
                         break;
                         }
                       case btnDOWN:
                         {
                         //lcd.print("DOWN  ");
                         lcd.clear();
                         V--;
                         Vmed=(V/200);
                         lcd.print(Vmed);
                         lcd.print(" [V]");
                         break;
                         }
                         
                        case btnNONE:
                           {
                         //lcd.print("NONE  ");
                         break;
                         }
                         
                       case btnSELECT:
                       {
                      
                       lcd.clear();  
                       lcd.print("Chosen Voltage: ");
                       lcd.setCursor(0,1);
                       lcd.print( Vmed);
                       lcd.print(" [V]");
                       delay(2000);
                      
                       
                       ok = false;
                       break;
                       
                      
                       }
                       

                     }
             
                   } 
                    
                  }   
                    
                    
                    
                    
                      
                    
              
             
               
               



                 void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world! ");
  delay(1000);
}

void loop() {
  // scroll 13 positions (string length) to the left 
  // to move it offscreen left:
  for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft(); 
    // wait a bit:
    delay(150);
  }

  // scroll 29 positions (string length + display length) to the right
  // to move it offscreen right:
  for (int positionCounter = 0; positionCounter < 29; positionCounter++) {
    // scroll one position right:
    lcd.scrollDisplayRight(); 
    // wait a bit:
    delay(150);
  }
  
    // scroll 16 positions (display length + string length) to the left
    // to move it back to center:
  for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft(); 
    // wait a bit:
    delay(150);
  }
  
  // delay at the end of the full loop:
  delay(1000);

}

                
                
                

    

