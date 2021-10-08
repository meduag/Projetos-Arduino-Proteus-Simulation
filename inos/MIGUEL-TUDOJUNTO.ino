
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
int currentMenuItem = 0;
int state = 0;
int lastState = 0;



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
 if (adc_key_in > 1000){
 return btnNONE; 
 state = 5;
 }
 
 else if (adc_key_in < 50){  
 return btnRIGHT;  
 state = 0;
 }
 
if (adc_key_in < 250){
 return btnUP;
state = 1; 
 }
 
if (adc_key_in < 450){  
 return btnDOWN; 
 state = 2;
 }
 
 if (adc_key_in < 650){ 
 return btnLEFT;
state = 3; 
 }
 
 if (adc_key_in < 850){ 
 return btnSELECT;
state = 4; 
 }
  
  else {
  return btnNONE;  // when all others fail, return this...
  state = 5;
  }
}





          void setup(){
  
                    pinMode(ss, OUTPUT);
                    SPI.begin(); // começa a transmissão SPI
                    SPI.setBitOrder(MSBFIRST);
                       
                     lcd.begin(16, 2);              // start the library
                     lcd.setCursor(0,0);
                     lcd.print("WELCOME!"); 
                     delay(1000);
                     lcd.clear();
                    
                 
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
            
            
            
                                //Print a basic header on Row 1.
                    void clearPrintTitle() {
                     lcd.clear();
                     lcd.setCursor(0,0);
                     lcd.print(" HACKSHED.CO.UK ");
                     lcd.setCursor(0,1); 
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
                    
                    
                    
                    
                      
                    
              
             
               
               

    void loop (){
  
      // Escolhe qual tipo de onda se usará:
      /* 1 - Senoidal
         2 - Cossenoidal
         3 - Quadrada
           3.1 - Quadrada com poço decaindo - começando de cima: (r,-,-,r,+,r,+)
           3.2 - Quadrada com o poço subindo - (começando de baixo: (r,+,+,r,-,r,-)
           3.3 - Quadrada com poços - começando de cima: (r,-,-,r,+,+)
           3.4 - Quadrada Positiva - começando de cima: (r,-,r,+)
           3.5 - Quadrada Negativa - começando de baixo: (r,+,r,-)
         4 - Exponencial
         5 - Triangular  
      
      // Recebe a tensão escolhida - menor que 10, caso contrario repete o valor
      
      // Recebe a resistencia escolhida - menor ou igual a tensão, caso contrario repete o valor
         
         A corrente é de x amperes, deseja usar uma corrente menor? Se sim...
      
      // Recebe a corrente escolhida - menor que a corrente x
      
      // Recebe a frenquencia/periodo escolhido
      
      */

                    mainMenu();
                    
              }
                    
              
              void mainMenu(){
                
                    read_LCD_buttons();
                   
                    //If we are out of bounds on th menu then reset it.
                     if (currentMenuItem < 0 || currentMenuItem > 4) {
                      currentMenuItem = 0; 
                     }
                    
                      //If we have changed Index, saves re-draws.
                      if (state != lastState) {
                         if (state == 1) {
                            //If Up
                             currentMenuItem = currentMenuItem - 1; 
                             displayMenu(currentMenuItem);
                         } else if (state == 2) {
                            //If Down
                             currentMenuItem = currentMenuItem + 1;  
                             displayMenu(currentMenuItem);
                         } else if (state == 3) {
                            //If Selected
                            selectMenu(currentMenuItem); 
                         }
                         //Save the last State to compare.
                         lastState = state;
                      } 
                      //Small delay
                     delay(5);
                    }
                    
                    //Display Menu Option based on Index.
                    void displayMenu(int adc_key_in) {
                        switch (adc_key_in) {
                         case 1:
                           //clearPrintTitle();
                           lcd.print ("1 - Voltage");
                           break;
                         case 2:
                           //clearPrintTitle();
                           lcd.print ("2 - Current");
                           break;
                          case 3:
                           //clearPrintTitle();
                           lcd.print ("3 - Impedance");
                           break;
                         case 4:
                           //clearPrintTitle();
                           lcd.print ("4 - Frequency");
                           break;
                       }
                    }
                    
                    //Show the selection on Screen.
                  void selectMenu(int adc_key_in) {
                    switch (adc_key_in) {
                       case 1:
                         //lcd.clear;
                         lcd.print ("Selected Opt 1");
                         Escolhe_Voltagem();
                         break;
                       case 2:
                         //lcd.clear;
                         lcd.print ("Selected Opt 2");
                         Escolhe_Corrente();
                         break;
                        case 3:
                         //lcd.clear;
                         lcd.print ("Selected Opt 3");
                         //Call the function that belongs to Option 3
                         break;
                       case 4:
                         //lcd.clear();
                         lcd.print ("Selected Opt 4");
                         //Call the function that belongs to Option 4
                         break;
                     }
                  }
                    
                    
               /*      lcd.clear();
                     lcd.print("Please insert ");
                     delay(2000);
                     lcd.clear();
                     lcd.print("the voltage");
                     lcd.setCursor(0,1);
                     lcd.print(" value:");
                     delay(2000);
                     lcd.clear();
                     lcd.print("Standard Voltage:");
                     lcd.setCursor(0,1);
                     lcd.print(" 0 V");
                     delay(2000);
                        
                
                                  
                Escolhe_Voltagem();
                
                
                     lcd.clear();
                     lcd.print("Please insert ");
                     delay(2000);
                     lcd.clear();
                     lcd.print("the current");
                     lcd.setCursor(0,1);
                     lcd.print(" value:");
                     delay(2000);
                     lcd.clear();
                     lcd.print("Standard Amper.:");
                     lcd.setCursor(0,1);
                     lcd.print(" 0 A");
                     delay(2000);
                     
                     
                Escolhe_Corrente();*/
                
                
                

    

