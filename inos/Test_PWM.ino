#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 14, 15, 16, 17, 18);

int PWMt = 1;
const int PWM_Out= 3;
const int BT_In_Up=  2;
const int BT_In_Down=  2;

void setup()   {  
  lcd.begin(16, 2);
  
  lcd.setCursor(0, 0);
  lcd.print("PWM --- ----");
  
  pinMode(PWM_Out, OUTPUT);
  pinMode(7, INPUT);
}

void loop()                     
{
  if (digitalRead(BT_In_Up) == HIGH) {
    
    if(PWMt > 255){
      PWMt = 1;
    }else{
      
      PWMt = PWMt + 1
    }
    
    
    
  } else if (digitalRead(BT_In) == HIGH){
    
    if(PWMt < 1){
      PWMt = 1;
    }else{
      PWMt = PWMt - 1
    }
    
    //analogWrite(redPin, 30);
  }
  
  analogWrite(PWM_Out, PWMt);
  lcd.setCursor(0, 1);
  lcd.print("PWM: ");
  lcd.setCursor(5, 1);
  lcd.print(PWMt);
  delay(10);
}

