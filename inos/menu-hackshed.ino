#include <LiquidCrystal.h>

// Initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7); // define os pinos utilizados

//States for the menu.
int currentMenuItem = 0;
int lastState = 0;
int state = 0;
int y = 0;
y = read_LCD_buttons();
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

void setup() {
  //Set the characters and column numbers.
  lcd.begin(16, 2);
  //Print default title.
  clearPrintTitle();
}

int read_LCD_buttons(){

 int x = analogRead (0);
 //Set the Row 0, Col 0 position.
 lcd.setCursor(0,0);

 //Check analog values from LCD Keypad Shield
 if (x < 100) {
   return 0;
   //Right
   
 } else if (x < 200) {
    return 1;
   //Up
   state = 1;
   
 } else if (x < 400){
   return 2;
   //Down
   state = 2;
   
 } else if (x < 600){
   return 4;
   //Left
   
 } else if (x < 800){
   return 3;
   //Select
   
   state = 3;
 }
}

void loop() {
 //Call the main menu.
 mainMenu();
}

void mainMenu() {
 //State = 0 every loop cycle.
 int state = 0;
 //Refresh the button pressed.

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
y = read_LCD_buttons();

void displayMenu(int y) {
    switch (y) {
     case 1:
       clearPrintTitle();
       lcd.print ("1 - Voltage");
       break;
     case 2:
       clearPrintTitle();
       lcd.print ("2 - Current");
       break;
      case 3:
       clearPrintTitle();
       lcd.print ("3 - Impedance");
       break;
     case 4:
       clearPrintTitle();
       lcd.print ("4 - Frequency");
       break;
   }
}

//Print a basic header on Row 1.
void clearPrintTitle() {
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print(" PARAMETERS ");
 lcd.setCursor(0,1); 
}

//Show the selection on Screen.
void selectMenu(int x) {
  switch (x) {
     case 1:
       clearPrintTitle();
       lcd.print ("Selected Opt 1");
       //Call the function that belongs to Option 1
       break;
     case 2:
       clearPrintTitle();
       lcd.print ("Selected Opt 2");
       //Call the function that belongs to Option 2
       break;
      case 3:
       clearPrintTitle();
       lcd.print ("Selected Opt 3");
       //Call the function that belongs to Option 3
       break;
     case 4:
       clearPrintTitle();
       lcd.print ("Selected Opt 4");
       //Call the function that belongs to Option 4
       break;
   }
}
