
#include "stdlib.h"
#include "math.h"
#include "SPI.h" 
#include <LiquidCrystal.h>


LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7); // define os pinos utilizados

//States for the menu
int currentMenuItem = 0;
int lastState = 0;

void setup(){
  // set the characters and columm numbers
  lcd.begin(16, 2);
  // print default title
  clearPrintTitle();
}

void loop(){
  //call the main menu
  mainMenu();
}

void mainMenu(){
  //state = 0 every loop cycle.
  int state = 0;
  //refresh the button pressed.
  int x = analogRead(0);
  //set the row 0, Col 0 position.
  lcd.setCursor(0,0);
  
  //check analog values from LCD keypad shield
  if (x<100){
    //right
  } else if(x < 200){
    //up
    state = 1;
  } else if(x < 400){
    //down
    state = 2;
  } else if(x < 600){
    //left
  } else if(x < 800){
   //select
    state = 3;
  }
 
 //if we are out of bounds on th menu then reset it.
  if(currentMenuItem < 0 || currentMenuItem > 4){
   currentMenuItem = 0;
  }
 
 //if we have changed index, saves re-draws.
   if(state != lastState){
    if(state == 1){
     // if up
      currentMenuItem = currentMenuItem - 1;
      displayMenu(CurrentMenuItem);
    }else if(state == 2){
     // if down
      currentMenuItem = currentMenuItem + 1;
     displayMenu(CurrentMenuItem);
    }else if (state == 3){
     // if selected
    selectMenu(currentMenuItem);
    }
   //save the last state to compare
   lastState = state;
   }
  
  delay(5);
}

//display menu option based on index.
void displayMenu(int x){
}
  
