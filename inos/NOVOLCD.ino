//Sample using LiquidCrystal library
#include <LiquidCrystal.h>

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
int lastState = 0;



float Vmed;
float Imed;
float V = 0; // valores que serão usados para começar a interface, 10 volts, 30 ohms e 30miliAmperes
float R = 0;
float I = 0;
float Tensao;
float Res; 
float Corrente;

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5


int LE_ANTIGO()
{
   adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  

 // For V1.0 comment the other threshold and use the one below:
/*
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   
*/


 return btnNONE;  // when all others fail, return this...
}
  

// read the buttons
void read_LCD_buttons()
{
 int state = 0; 
int adc_key_in = analogRead(0);  // read the value from the sensor 
lcd.setCursor(0,0);
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) {} //return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 
else if  (adc_key_in < 50){} //return btnRIGHT;  

else if (adc_key_in < 250){  
  state = 1;
 //return 1;
 }
 
else if (adc_key_in < 450){
  state = 2; 
  //return 2;
}
  
else if (adc_key_in < 650){} // return btnLEFT; 
 
else if (adc_key_in < 850){
 state = 3;
 //return 3;  
 }




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


 //return btnNONE;  // when all others fail, return this...
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
  
             
         /* aqui fazemos uma regra de 3:
         10 - 4095 
         Tensao - x 
         outputvalue é igual x, que nada mais é do que a tensão convertida pro padrão do circuito
         */ 
         
        
          float TensaoConv; // Essa variável vai receber a tensão escolhida pelo usuário e converter pro padrão do circuito. O sinal digital tem um limite de 4095 bits e o analógico não pode passar de 10 volts
          
          TensaoConv = ((Tensao*4095)/10); 
          
          
                 outputValue = TensaoConv;
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
  
                 
                 float TensaoConv = ((Tensao*4095)/10); 
          
                 outputValue = TensaoConv;
                    //outputValue = 4095;
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
                  
                    outputValue = (TensaoConv)/2;
                     //outputValue = 2048;
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
                    
                    outputValue = (TensaoConv/2);
                     //outputValue = 2048;
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
    
    
          void Onda_Senoidal(){

            float TensaoConv = ((Tensao*4095)/10);
            
                    /*for (int x=30; x<=150; x++) {
                      sinVal = (sin(x*(3.1412/180)));
                      OsVal = int(sinVal*4095);
                     outputValue = OsVal;    */
                     
                     for (int x=30; x<=150; x++) {
                      sinVal = (sin(x*(3.1412/180)));
                      OsVal = int(sinVal*TensaoConv);
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
                  
                  /* for (int x=330; x>210; x--) {
                      sinVal = (sin(x*(3.1412/180)));
                      OsVal = int(sinVal*4095);
                     outputValue = OsVal;*/ 
                     
                     for (int x=330; x>210; x--) {
                      sinVal = (sin(x*(3.1412/180)));
                      OsVal = int(sinVal*TensaoConv);
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
              
               lcd.clear();
                     lcd.print("Please insert ");
                     delay(1000);
                     lcd.clear();
                     lcd.print("the current");
                     lcd.setCursor(0,1);
                     lcd.print(" value:");
                     delay(2000);
                     lcd.clear();
                     lcd.print("Standard Amp.:");
                     lcd.setCursor(0,1);
                     lcd.print(" 0 mA");
                     delay(2000);
                  
                      bool ok = true;
                      while(ok){
               
                   
                    lcd_key = LE_ANTIGO();  // Lê os botões
                     
                  
                     switch ( lcd_key)               // Para cada botão escolhido, uma ação acontece
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
                         lcd.print(" [mA]");
                         break;
                         }
                       case btnDOWN:
                         {
                         //lcd.print("DOWN  ");
                         lcd.clear();
                         I--;
                         Imed=(I/200);
                         lcd.print(Imed);
                         lcd.print(" [mA]");
                         break;
                         }
                         
                        /*case 5:
                           {
                         //lcd.print("NONE  ");
                         break;
                         }*/
                         
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
                    
                     lcd.clear();
                     lcd.print("Please insert ");
                     delay(1000);
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
                 
                bool ok = true;
                while(ok){
               
                   
                lcd_key = LE_ANTIGO();  // read the buttons
                     
                  // read_LCD_buttons();
                     switch ( lcd_key)               // depending on which button was pushed, we perform an action
                     {
                       case btnRIGHT:
                         {
                        // lcd.print("RIGHT ");
                         break;
                         }
                       case btnLEFT:
                         {
                         //lcd.print("LEFT   ");
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
                         
                        /*case 5:
                           {
                         //lcd.print("NONE  ");
                         break;
                         }*/
                         
                       case btnSELECT:
                       {
                      
                       lcd.clear();  
                       lcd.print("Chosen Voltage: ");
                       lcd.setCursor(0,1);
                       lcd.print( Vmed);
                       lcd.print(" [V]");
                       delay(2000);
                       Vmed = Tensao;
                      
                       
                       ok = false;
                       break;
                       
                      
                       }
                       

                     }
             
                   } 
                    
                  }   
                  
                  
                  //Display Menu Option based on Index.
void displayMenu(int  adc_key_in) {
  //lcd_key = read_LCD_buttons();
    switch ( adc_key_in) {
     case 1:
       clearPrintTitle();
       lcd.print ("-> Voltage ");
       break;
     case 2:
       clearPrintTitle();
       lcd.print ("-> Current ");
       break;
      case 3:
       clearPrintTitle();
       lcd.print ("-> Frequency ");
       break;
     case 4:
       clearPrintTitle();
       lcd.print ("-> Generate Waveform");
       break;
   }
}


//Print a basic header on Row 1.
void clearPrintTitle() {
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print(" Edit Parameters:  ");
 lcd.setCursor(0,1); 
}



void  Escolhe_Forma_de_Onda(int  adc_key_in) {
  //lcd_key = read_LCD_buttons();
    switch ( adc_key_in) {
     case 1:
       clearPrintTitle();
       lcd.print ("-> Quadrada ");
       break;
     case 2:
       clearPrintTitle();
       lcd.print ("-> Escada ");
       break;
      case 3:
       clearPrintTitle();
       lcd.print ("-> Senoidal ");
       break;
     case 4:
       clearPrintTitle();
       lcd.print ("-> Exponencial ");
       break;
   }
}





//Show the selection on Screen.
void selectMenu(int  adc_key_in) {
 // lcd_key = read_LCD_buttons();
  switch ( adc_key_in) {
     case 1:
       clearPrintTitle();
       lcd.print ("Selected Opt 1");
       delay(1000);
       Escolhe_Voltagem();
       break;
       
     case 2:
       clearPrintTitle();
       lcd.print ("Selected Opt 2");
       delay(1000);
       Escolhe_Corrente();
       break;
       
      case 3:
       clearPrintTitle();
       lcd.print ("Selected Opt 3");
       //Call the function that belongs to Option 3
       break;
    
     case 4:
       clearPrintTitle();
       lcd.print ("Selected Opt 4");
       Escolhe_Forma_de_Onda(adc_key_in);//Call the function that belongs to Option 4
       break;
   }
   
}
   
   
   void selectWave(int adc_key_in) {
 // lcd_key = read_LCD_buttons();
  switch ( adc_key_in) {
     case 1:
       clearPrintTitle();
       lcd.print ("Onda Quadrada");
       delay(1000);
       Onda_Quadrada();
       break;
       
     case 2:
       clearPrintTitle();
       lcd.print ("Onda Escada");
       delay(1000);
       Onda_Escada();
       break;
       
      case 3:
       clearPrintTitle();
       lcd.print ("Onda Senoidal");
       Onda_Senoidal();
       break;
    
     case 4:
       clearPrintTitle();
       lcd.print ("Onda Exponencial");
       Onda_Exponencial();
       break;
   }
}
   
   
}
 
void loop()
{
 
read_LCD_buttons();
displayMenu( adc_key_in);
selectMenu( adc_key_in);
selectWave(adc_key_in)

 
}
                
                
