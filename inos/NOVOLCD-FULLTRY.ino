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
int currentMenuItem2 = 0;
int lastState2 = 0;
int i;



float Vmed=0;
float Imed=0;
float V = 0; // valores que serão usados para começar a interface, 10 volts, 30 ohms e 30miliAmperes
float R = 0;
float I = 0;
float Tensao;
float Res=0; 
float Corrente=0;
float TensaoConv=0;


// select the pins used on the LCD panel
LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
int adc_key_in2  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5


int LE_ANTIGO()
{
   adc_key_in = analogRead(0);      // lê os valores do sensor  
 // valores lidos de cada botão: 0, 144, 329, 504, 741
 
 if (adc_key_in > 1000) return btnNONE; // faixa de valores para cada botão
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  



 return btnNONE;  //se todos falharem, retorna esse
}
  


void read_LCD_buttons() // função que lê os valores 
{
 int state = 0; 
int adc_key_in = analogRead(0);  // lê valores do sensor
lcd.setCursor(0,0);
 
 if (adc_key_in > 1000) {} 
 
 
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




//loop para quando acabar as opções, recomeçar do primeiro
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
     //salva o ultimo estado para comparar
     lastState = state;
  } 


 //return btnNONE;  
}



void read_LCD_Wave() // função igual a LCD_Read mas que varia parâmetros para mudar os tipos de onda
{
 int state2 = 0; 
int adc_key_in2 = analogRead(0);  
lcd.setCursor(0,0);
 
 if (adc_key_in2 > 1000) {} //return btnNONE; 
 
else if  (adc_key_in2 < 50){} //return btnRIGHT;  

else if (adc_key_in2 < 250){  
  state2 = 1;
 //return 1;
 }
 
else if (adc_key_in2 < 450){
  state2 = 2; 
  //return 2;
}
  
else if (adc_key_in2 < 650){} // return btnLEFT; 
 
else if (adc_key_in2 < 850){
 state2 = 3;
 //return 3;  
 }





 if (currentMenuItem2 < 0 || currentMenuItem2 > 4) {
  currentMenuItem2 = 0; 
 }

  //If 
  if (state2 != lastState2) {
     if (state2 == 1) {
        //If Up
         currentMenuItem2 = currentMenuItem2 - 1; 
         Escolhe_Forma_de_Onda(currentMenuItem2);
     } else if (state2 == 2) {
        //If Down
         currentMenuItem2 = currentMenuItem2 + 1;  
         Escolhe_Forma_de_Onda(currentMenuItem2);
     } else if (state2 == 3) {
        //If Selected
        selectWave(currentMenuItem2); 
     }
     
     lastState2 = state2;
  } 


 //return btnNONE;  
}

   void setup(){
     int i;
  
                    pinMode(ss, OUTPUT);
                    SPI.begin(); // começa a transmissão SPI
                    SPI.setBitOrder(MSBFIRST);
                       
                    lcd.begin(16, 2);              
                    lcd.setCursor(0,0);
                    lcd.print("WELCOME!"); 
                    delay(2000);
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Edit Parameters:");
                   
                 
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
                       lcd.print(" [mA]");
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
                   
                   Vmed = Tensao;   
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
       lcd.print ("-> Waveform");
        /*for(i=0;i<15;i++){
         lcd.setCursor(0,0);
         lcd.print("                  ");
         lcd.setCursor(0,0);
          lcd.print("Edit Parameters:  ");
           
                   lcd.scrollDisplayLeft();
                   delay(250); 
                  }
                  delay(1000);
                  
      for(i=15;i>0;i--){    
        lcd.scrollDisplayRight();
                   delay(250); 
      }*/
                 
                  
       break;
   }
}


//Cabeçalho
void clearPrintTitle() {
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Edit Parameters:  ");
 lcd.setCursor(0,1); 
}

void clearPrintTitle2() {
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Choose Waveform:  ");
 lcd.setCursor(0,1); 
}



void  Escolhe_Forma_de_Onda(int adc_key_in2) {
  //lcd_key = read_LCD_buttons();
  
 // clearPrintTitle();
 // delay(3000);
  //read_LCD_Wave();
  
    switch ( adc_key_in2) {
     case 1:
       clearPrintTitle2();
       lcd.print ("-> Quadrada ");
       break;
     case 2:
       clearPrintTitle2();
       lcd.print ("-> Escada ");
       break;
      case 3:
       clearPrintTitle2();
       lcd.print ("-> Senoidal ");
       break;
     case 4:
       clearPrintTitle2();
       lcd.print ("-> Triangular ");
       break;
   }
  
}

void TRY(){
  
read_LCD_buttons();

displayMenu( adc_key_in);
selectMenu( adc_key_in);

  }


 
void TRY2(){
  
  read_LCD_Wave();  
Escolhe_Forma_de_Onda(adc_key_in2);
selectWave( adc_key_in2);
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
       
       break;
    
     case 4:
       clearPrintTitle();
       lcd.print ("Selected Opt 4");
       clearPrintTitle2();
       while(1) {
         read_LCD_Wave();  
       Escolhe_Forma_de_Onda(adc_key_in2);
        selectWave( adc_key_in2);
       
       }
       //break;
       
   }
}
   

   
   
   void selectWave(int adc_key_in2) {
 // lcd_key = read_LCD_buttons();
  switch ( adc_key_in2) {
     case 1:
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print ("Onda Gerada:");
       lcd.setCursor(0,1);
       lcd.print ("-> Quadrada");
       delay(1000);
       while(1){Onda_Quadrada();}
       break;
       
     case 2:
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print ("Onda Gerada:");
       lcd.setCursor(0,1);
       lcd.print ("-> Escada");
       delay(1000);
       while(1){Onda_Escada();}
       break;
       
      case 3:
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print ("Onda Gerada:");
       lcd.setCursor(0,1);
       lcd.print ("-> Senoidal");
       delay(1000);
      while(1){ Onda_Senoidal();}
       break;
    
     case 4:
       
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print ("Onda Gerada:");
       lcd.setCursor(0,1);
       lcd.print ("-> Triangular");
       delay(1000);
       while(1){Onda_Triangular();}
       break;
   }
}


void Onda_Quadrada(){
          
             
         /* aqui fazemos uma regra de 3:
         5 - 4095 
         Tensao - x 
         outputvalue é igual x, que nada mais é do que a tensão convertida pro padrão do circuito
         */ 
         
        
          //float TensaoConv; // Essa variável vai receber a tensão escolhida pelo usuário e converter pro padrão do circuito. O sinal digital tem um limite de 4095 bits e o analógico não pode passar de 10 volts
          
          TensaoConv = ((Tensao*4095)/5); 
          
          
                 //outputValue = TensaoConv;
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
  
  
        void Onda_Escada (){
  
                 
                  TensaoConv = ((Tensao*4095)/5); 
          
                 //outputValue = TensaoConv;
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
                  
                    //outputValue = (TensaoConv)/2;
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
                    
                    //outputValue = (TensaoConv/2);
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
    
    
          void Onda_Senoidal(){

                 TensaoConv = ((Tensao*4095)/5);
            
                    for (int x=30; x<=150; x++) {
                      sinVal = (sin(x*(3.1412/180)));
                      OsVal = int(sinVal*4095);
                     outputValue = OsVal;    
                    
                     /*for (int x=30; x<=150; x++) 
                      sinVal = (sin(x*(3.1412/180)));
                      OsVal = int(sinVal*TensaoConv);
                      outputValue = OsVal;*/
                     
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
                     
                    /* for (int x=330; x>210; x--) 
                      sinVal = (sin(x*(3.1412/180)));
                      OsVal = int(sinVal*TensaoConv);
                      outputValue = OsVal;*/
                     
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
  

            void Onda_Exponencial(){

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
                                
   
   void Onda_Triangular() 
{

  

 for (int x=0; x<=50; x++) {
   
   outputValue =(204,75*x) ;
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


 for (int x=49; x>=0; x--) {
   
    outputValue =(204,75*x) ;
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



void loop()
{
 

TRY();
 
}
                
                
