#include <LiquidCrystal.h> //importando biblioteca de LiquidCrustal
#include <Wire.h>//importando biblioteca para com I2C

LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7); //config pines en los que se conecta

int lcd_key     = 0;       // define os valores representativos de cada botão
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5


//Variables Globales
const int ledPin = 13;
char mensaje;
int key_stado = 0;
char dado ;
int sel = 0;

//var aux
int entrar = 0;
int inicl = 0;
String Opc1 = "N";
String Opc2 = "Nn";
int TempoBoton = 150;

//Limnites de tempos para onda quadrada
int Lim_T1 = 1000;

int T1 = 0, T2 = 0, T3 = 0, T4 = 0, mA = 0, Fq = 0;

char* Menu_Pro[] = {"Electro Diagnost", "Terapeutica     "
                   };

char* Menu_EleDiag[] = {"Reobase         ", "Cronaxia        ", "Acomodacao      ", "Volt Menu Ppal  "
                       };

char* Menu_T_WF[] = {"Terapia WF Quadr", "Terapia WF Seno ", "Terapia WF Expon", "Volt Menu Ppal  "
                    };

//Colocar los menus para las otras formas de onda
char* Menu_WFQ_para[] = {"Tempo 1 u_S     ", "Tempo 2 u_S     ", "Tempo 3 u_S     ",
                         "Tempo 4 u_S     ", "Freq Hz         ", "Corrente mA     ", "Volt Mnu Terapia"
                        };


void setup()
{
  Wire.begin();//INICIAMOS EL WIRE (I2C)
  Serial.begin(9600);// INICIAMOS EL SERIAL PARA MOSTRAR LOS MENSAJES
  Serial.println("Prototipo 1 Iniciando");//IMPRIMIMOS DE QUE HEMOS PEDIDO EL MESNAJE AL DISPOSITIVO EN EL CANAL 1
  lcd.begin(16, 2);
  //Iniciar_ProtMensa();///mensaje inicial en la LCD
}




void loop() {
  //colocar los menus o el menú
  Menu_PrinPro();


}//fin void loop















/***************************************************************************************************************************/
///////// Menu Principal del Prototipo ////////////////////////
/***************************************************************************************************************************/


void Menu_PrinPro() {
  Opc1 = "N";
  Opc2 = "Nn";
  sel = 0;
  entrar = 1;

  lcd.setCursor(0, 0);
  lcd.print("   Menu Ppal    ");

  if (inicl == 0) {
    lcd.setCursor(0, 1);
    lcd.print("Press Up or Down");
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print("And then Select ");
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print(Menu_Pro[sel]);
    delay(1000);
    inicl = 1;
  } else {
    lcd.setCursor(0, 1);
    lcd.print(Menu_Pro[sel]);
  }

  while (entrar == 1) {
    lcd_key = 0;
    lcd_key = read_LCD_buttons();  // Lê os botões
    delay(TempoBoton);

    switch (lcd_key)// Para cada botão escolhido, uma ação acontece
    {
      case btnUP:
        {
          sel = sel + 1;
          if (sel == 2 ) {
            sel = 0;
          }
          lcd.setCursor(0, 1);
          lcd.print(Menu_Pro[sel]);
          break;
        }
      case btnDOWN:
        {
          sel = sel - 1;
          if (sel < 0 ) {
            sel = 1;
          }
          lcd.setCursor(0, 1);
          lcd.print(Menu_Pro[sel]);
          break;
        }
      case btnSELECT:
        {
          //lcd.clear();
          //lcd.setCursor(0, 0);
          //lcd.print(Menu_Pro[sel]);
          Opc1 = Menu_Pro[sel];
          Opc2 = Menu_Pro[0];

          if (Opc1.equals(Opc2)) {
            //Llamar al menu de electrodiagnostico
            sel = 0;
            entrar = 0;
            Menu_EDiag();
          } else {
            //Llamar al menu de Terapia
            sel = 0;
            entrar = 0;
            Menu_Terapia();
          }
          break;
        }
    }//fin case

  }//fin while

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************    Electrodiagnostico ou Terapeutica   ***********************************************************/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////// Menu electro diagnostico ////////////////////
void Menu_EDiag() {
  lcd.clear();
  sel = 0;

  while (entrar == 0) {
    lcd.setCursor(0, 0);
    lcd.print("  Menu E_Diag   ");
    lcd.setCursor(0, 1);
    lcd.print(Menu_EleDiag[sel]);

    lcd_key = 0;
    lcd_key = read_LCD_buttons();  // Lê os botões
    delay(TempoBoton);

    switch (lcd_key)// Para cada botão escolhido, uma ação acontece
    {
      case btnUP:
        {
          sel = sel + 1;
          if (sel >= 4 ) {
            sel = 0;
          }
          lcd.setCursor(0, 1);
          lcd.print(Menu_EleDiag[sel]);
          break;
        }
      case btnDOWN:
        {
          sel = sel - 1;
          if (sel < 0 ) {
            sel = 3;
          }
          lcd.setCursor(0, 1);
          lcd.print(Menu_EleDiag[sel]);
          break;
        }
      case btnSELECT:
        {
          //llamar las funciones como en el menu prioncipal para los valores de Electrodiagnostico
          //por enquanto cuelve al menu principal
          //lcd.print("Back Menu ppal  ");
          Opc1 = Menu_EleDiag[sel];
          Opc2 = Menu_EleDiag[3];///Para poder regresar al menu principal

          if (Opc1.equals(Opc2)) {
            //Me regreso al menú principal
            entrar = 1;
            sel = 0;
            Menu_PrinPro();
          } else {
            entrar = 1;
            sel = 0;
            ///////////////////////////// Ojo aqui va la funcion de los parametros para Electro diagnostico

            Menu_PrinPro();////Colocar aqui la otra funcion
          }
          break;
        }
    }//fin switch case

  }//fin while


}//fin funcion ediag


///////////////////////// Menu electro diagnostico ////////////////////
void Menu_Terapia() {
  sel = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Menu Terapia  ");
  lcd.setCursor(0, 1);
  lcd.print(Menu_T_WF[sel]);


  while (entrar == 0) {
    lcd_key = 0;
    lcd_key = read_LCD_buttons();  // Lê os botões
    delay(TempoBoton);

    switch (lcd_key)// Para cada botão escolhido, uma ação acontece
    {
      case btnUP:
        {
          sel = sel + 1;
          if (sel >= 4 ) {
            sel = 0;
          }
          lcd.setCursor(0, 1);
          lcd.print(Menu_T_WF[sel]);
          break;
        }
      case btnDOWN:
        {
          sel = sel - 1;
          if (sel < 0 ) {
            sel = 3;
          }
          lcd.setCursor(0, 1);
          lcd.print(Menu_T_WF[sel]);
          break;
        }
      case btnSELECT:
        {
          //llamar las funciones como en el menu principal para las forma de onda Q, S , E
          //por enquanto solo entra a forma de onda cuadrada
          Opc1 = Menu_T_WF[sel];
          Opc2 = Menu_T_WF[3];///Para poder regresar al menu principal

          if (Opc1.equals(Opc2)) {
            //Me regreso al menú principal
            Menu_PrinPro();
          } else {
            sel = 0;
            entrar = 0;
            Menu_T_WFQ_parametros();
          }
          break;
        }
    }
  }//fin while
}//Fin funcion terapia





/***************************************************************************************************************************/
//////////////////////////////////        Funcion para selecionar do menu ppal           ////////////////////////////////////
/***************************************************************************************************************************/

/***************************************************************************************************************************/
///////// Menu Parametros Forma de  Quadrada////////////////////////
/***************************************************************************************************************************/

void Menu_T_WFQ_parametros() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Menu Confg WFQ ");
  lcd.setCursor(0, 1);
  lcd.print(Menu_WFQ_para[sel]);


  while (entrar == 0) {
    lcd_key = 0;
    lcd_key = read_LCD_buttons();  // Lê os botões
    delay(TempoBoton);

    switch (lcd_key)// Para cada botão escolhido, uma ação acontece
    {
      case btnUP:
        {
          sel = sel + 1;
          if (sel > 6 ) {
            sel = 0;
          }
          lcd.setCursor(0, 1);
          lcd.print(Menu_WFQ_para[sel]);
          break;
        }
      case btnDOWN:
        {
          sel = sel - 1;
          if (sel < 0 ) {
            sel = 5;
          }
          lcd.setCursor(0, 1);
          lcd.print(Menu_WFQ_para[sel]);
          break;
        }
      case btnSELECT:
        {
          entrar = 0;
          Opc2 = Menu_WFQ_para[sel];
          Opc1 = Menu_WFQ_para[6];

          if (Opc1.equals(Opc2)) {
            //Me regreso al menú principal
            Menu_Terapia();
          } else {
            sel = 0;
            entrar = 0;
            MudarValorParametro(Opc2);
          }
          break;
        }
    }

  }//fin while
}//Fin funcion de Mudar los datos de la onda quadrada


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// Muda los valores de los parametros de la forma de onda Quadrada
void MudarValorParametro(String Mv) {
  Opc1 = Menu_WFQ_para[0];
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Parametros WFQ ");
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print(Mv);
  delay(500);


  int entrar2 = 0;

  //////////////////////////////////////////////////////////////////////////------------------ T1
  if (Opc1.equals(Mv)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(Mv);

    lcd.setCursor(0, 1);
    lcd.print("T1 =    ");
    lcd.setCursor(5, 1);
    lcd.print(T1);
    lcd.print(" uS   **");
    delay(500);

    while (entrar2 == 0) {
      lcd_key = 0;
      lcd_key = read_LCD_buttons();  // Lê os botões
      delay(TempoBoton);

      switch (lcd_key)// Para cada botão escolhido, uma ação acontece
      {
        case btnUP:
          {
            T1 = T1 + 1;
            if (T1 >= Lim_T1 ) {
              T1 = 0;
            }
            lcd.setCursor(5, 1);
            lcd.print(T1);
            lcd.print(" uS   ++");
            break;
          }
        case btnDOWN:
          {
            T1 = T1 - 1;
            if (T1 < 0 ) {
              T1 = 0;
            }
            lcd.setCursor(5, 1);
            lcd.print(T1);
            lcd.print(" uS   --");
            break;
          }
        case btnSELECT:
          {
            entrar2 = 0;
            Menu_T_WFQ_parametros();
          }
      }//fin case
    }//Fin while
  } else {
    Opc1 = Menu_WFQ_para[1];
  }//fin if principal WFQ0

  //////////////////////////////////////////////////////////////////////////------------------ T2
  if (Mv.equals(Opc1)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(Mv);

    lcd.setCursor(0, 1);
    lcd.print("T2 =   ");
    lcd.setCursor(5, 1);
    lcd.print(T2);
    lcd.print(" uS   **");
    delay(500);

    while (entrar2 == 0) {
      lcd_key = 0;
      lcd_key = read_LCD_buttons();  // Lê os botões
      delay(TempoBoton);

      switch (lcd_key)// Para cada botão escolhido, uma ação acontece
      {
        case btnUP:
          {
            T2 = T2 + 1;
            if (T2 >= Lim_T1 ) {
              T2 = 0;
            }
            lcd.setCursor(5, 1);
            lcd.print(T2);
            lcd.print(" uS   ++");
            break;
          }
        case btnDOWN:
          {
            T2 = T2 - 1;
            if (T2 < 0 ) {
              T2 = 0;
            }
            lcd.setCursor(5, 1);
            lcd.print(T2);
            lcd.print(" uS   --");
            break;
          }
        case btnSELECT:
          {
            entrar2 = 0;
            Menu_T_WFQ_parametros();
          }
      }//fin case
    }//Fin while
  } else {
    Opc1 = Menu_WFQ_para[2];
  }//fin if principal WFQ0


  //////////////////////////////////////////////////////////////////////////------------------ T3
  if (Opc1.equals(Mv)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(Mv);

    lcd.setCursor(0, 1);
    lcd.print("T3 =   ");
    lcd.setCursor(5, 1);
    lcd.print(T3);
    lcd.print(" uS   **");
    delay(500);

    while (entrar2 == 0) {
      lcd_key = 0;
      lcd_key = read_LCD_buttons();  // Lê os botões
      delay(TempoBoton);

      switch (lcd_key)// Para cada botão escolhido, uma ação acontece
      {
        case btnUP:
          {
            T3 = T3 + 1;
            if (T3 >= Lim_T1 ) {
              T3 = 0;
            }
            lcd.setCursor(5, 1);
            lcd.print(T3);
            lcd.print(" uS   ++");
            break;
          }
        case btnDOWN:
          {
            T3 = T3 - 1;
            if (T3 < 0 ) {
              T3 = 0;
            }
            lcd.setCursor(5, 1);
            lcd.print(T3);
            lcd.print(" uS   --");
            break;
          }
        case btnSELECT:
          {
            entrar2 = 0;
            Menu_T_WFQ_parametros();
          }
      }//fin case
    }//Fin while
  } else {
    Opc1 = Menu_WFQ_para[3];
  }//fin if principal WFQ0

  //////////////////////////////////////////////////////////////////////////------------------ T4
  if (Opc1.equals(Mv)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(Mv);

    lcd.setCursor(0, 1);
    lcd.print("T4 =   ");
    lcd.setCursor(5, 1);
    lcd.print(T4);
    lcd.print(" uS   **");
    delay(500);

    while (entrar2 == 0) {
      lcd_key = 0;
      lcd_key = read_LCD_buttons();  // Lê os botões
      delay(TempoBoton);

      switch (lcd_key)// Para cada botão escolhido, uma ação acontece
      {
        case btnUP:
          {
            T4 = T4 + 1;
            if (T4 >= Lim_T1 ) {
              T4 = 0;
            }
            lcd.setCursor(5, 1);
            lcd.print(T4);
            lcd.print(" uS   ++");
            break;
          }
        case btnDOWN:
          {
            T4 = T4 - 1;
            if (T4 < 0 ) {
              T4 = 0;
            }
            lcd.setCursor(5, 1);
            lcd.print(T4);
            lcd.print(" uS   --");
            break;
          }
        case btnSELECT:
          {
            entrar2 = 0;
            Menu_T_WFQ_parametros();
          }
      }//fin case
    }//Fin while
  } else {
    Opc1 = Menu_WFQ_para[4];
  }//fin if principal WFQ0


  //////////////////////////////////////////////////////////////////////////------------------ Fq
  if (Opc1.equals(Mv)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(Mv);

    lcd.setCursor(0, 1);
    lcd.print("Fq =    ");
    lcd.setCursor(5, 1);
    lcd.print(Fq);
    lcd.print(" Hz   **");
    delay(500);

    while (entrar2 == 0) {
      lcd_key = 0;
      lcd_key = read_LCD_buttons();  // Lê os botões
      delay(TempoBoton);

      switch (lcd_key)// Para cada botão escolhido, uma ação acontece
      {
        case btnUP:
          {
            Fq = Fq + 1;
            if (Fq >= Lim_T1 ) {
              Fq = 0;
            }
            lcd.setCursor(5, 1);
            lcd.print(Fq);
            lcd.print(" Hz   ++");
            break;
          }
        case btnDOWN:
          {
            Fq = Fq - 1;
            if (Fq < 0 ) {
              Fq = 0;
            }
            lcd.setCursor(5, 1);
            lcd.print(Fq);
            lcd.print(" Hz   --");
            break;
          }
        case btnSELECT:
          {
            entrar2 = 0;
            Menu_T_WFQ_parametros();
          }
      }//fin case
    }//Fin while
  } else {
    Opc1 = Menu_WFQ_para[5];
  }//fin if principal WFQ0


  //////////////////////////////////////////////////////////////////////////------------------ mA
  if (Opc1.equals(Mv)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(Mv);

    lcd.setCursor(0, 1);
    lcd.print("A =    ");
    lcd.setCursor(5, 1);
    lcd.print(mA);
    lcd.print(" mA   **");
    delay(500);

    while (entrar2 == 0) {
      lcd_key = 0;
      lcd_key = read_LCD_buttons();  // Lê os botões
      delay(TempoBoton);

      switch (lcd_key)// Para cada botão escolhido, uma ação acontece
      {
        case btnUP:
          {
            mA = mA + 1;
            if (mA >= Lim_T1 ) {
              mA = 0;
            }
            lcd.setCursor(5, 1);
            lcd.print(mA);
            lcd.print(" mA   ++");
            break;
          }
        case btnDOWN:
          {
            mA = mA - 1;
            if (mA < 0 ) {
              mA = 0;
            }
            lcd.setCursor(5, 1);
            lcd.print(mA);
            lcd.print(" mA   --");
            break;
          }
        case btnSELECT:
          {
            entrar2 = 0;
            Menu_T_WFQ_parametros();
          }
      }//fin case
    }//Fin while
  } else {
    entrar2 = 0;
    Menu_T_WFQ_parametros();
  }//fin if principal WFQ0







  /*
    if (Opc1.equals(Mv)) {
      lcd.setCursor(0, 1);
      lcd.print("T2 = ");
      lcd.setCursor(5, 1);
      lcd.print(T2);
      lcd.print(" uS     ");

      lcd_key = 0;

      do {
        lcd_key = read_LCD_buttons();  // Lê os botões
        delay(TempoBoton);
        if (lcd_key == 1) {
          lcd.setCursor(5, 1);
          lcd.print(T2);
          lcd.print(" uS     ");
          T2 = T2 + 1;
        }
        if (lcd_key == 2 && T2 > 0) {
          lcd.setCursor(5, 1);
          lcd.print(T2);
          lcd.print(" uS     ");
          T2 = T2 - 1;
        }
        if (lcd_key == 4) {
          entrar2 = 1;
        } else {
          lcd_key = 0;
        }
      } while (entrar2 == 0);
    } else {
      Opc1 = Menu_WFQ[2];
    }


    if (Opc1.equals(Mv)) {
      lcd.setCursor(0, 1);
      lcd.print("T3 = ");
      lcd.setCursor(5, 1);
      lcd.print(T3);
      lcd.print(" uS     ");

      lcd_key = 0;

      do {
        lcd_key = read_LCD_buttons();  // Lê os botões
        delay(TempoBoton);
        if (lcd_key == 1) {
          lcd.setCursor(5, 1);
          lcd.print(T3);
          lcd.print(" uS     ");
          T3 = T3 + 1;
        }
        if (lcd_key == 2 && T3 > 0) {
          lcd.setCursor(5, 1);
          lcd.print(T1);
          lcd.print(" uS     ");
          T3 = T3 - 1;
        }
        if (lcd_key == 4) {
          entrar2 = 1;
        } else {
          lcd_key = 0;
        }
      } while (entrar2 == 0);
    } else {
      Opc1 = Menu_WFQ[3];
    }


    if (Opc1.equals(Mv)) {
      lcd.setCursor(0, 1);
      lcd.print("T4 = ");
      lcd.setCursor(5, 1);
      lcd.print(T4);
      lcd.print(" uS     ");
      lcd_key = 0;

      do {
        lcd_key = read_LCD_buttons();  // Lê os botões
        delay(TempoBoton);
        if (lcd_key == 1) {
          lcd.setCursor(5, 1);
          lcd.print(T4);
          lcd.print(" uS     ");
          T4 = T4 + 1;
        }
        if (lcd_key == 2 && T4 > 0) {
          lcd.setCursor(5, 1);
          lcd.print(T1);
          lcd.print(" uS     ");
          T4 = T4 - 1;
        }
        if (lcd_key == 4) {
          entrar2 = 1;
        } else {
          lcd_key = 0;
        }
      } while (entrar2 == 0);
    } else {
      Opc1 = Menu_WFQ[4];
    }


    if (Opc1.equals(Mv)) {
      lcd.setCursor(0, 1);
      lcd.print("A = ");
      lcd.setCursor(5, 1);
      lcd.print(mA);
      lcd.print(" mA    ");

      lcd_key = 0;

      do {
        lcd_key = read_LCD_buttons();  // Lê os botões
        delay(TempoBoton);

        if (lcd_key == 1) {
          lcd.setCursor(5, 1);
          lcd.print(mA);
          lcd.print(" mA    ");
          mA = mA + 1;
        }
        if (lcd_key == 2 && mA > 0) {
          lcd.setCursor(5, 1);
          lcd.print(mA);
          lcd.print(" mA    ");
          mA = mA - 1;
        }
        if (lcd_key == 4) {
          entrar2 = 1;
        } else {
          lcd_key = 0;
        }
      } while (entrar2 == 0);
    } else {
      Opc1 = Menu_WFQ[5];
    }


    if (Opc1.equals(Mv)) {
      lcd.setCursor(0, 1);
      lcd.print("Fq = ");
      lcd.setCursor(5, 1);
      lcd.print(Fq);
      lcd.print(" Hz    ");

      lcd_key = 0;

      do {
        lcd_key = read_LCD_buttons();  // Lê os botões
        delay(TempoBoton);
        if (lcd_key == 1) {
          lcd.setCursor(5, 1);
          lcd.print(Fq);
          lcd.print(" Hz    ");
          Fq = Fq + 1;
        }
        if (lcd_key == 2 && Fq > 0) {
          lcd.setCursor(5, 1);
          lcd.print(Fq);
          lcd.print(" Hz    ");
          Fq = Fq - 1;
        }
        if (lcd_key == 4) {
          entrar2 = 1;
        } else {
          lcd_key = 0;
        }
      } while (entrar2 == 0);
    } else {
      Opc1 = Menu_WFQ[6];
    }
    */

  /* if (Opc1.equals(Mv)) {//volver al menu anterior
     Menu_T_WFQ_parametros();
   }*/
  lcd.setCursor(0, 0);
  lcd.print("No hay considenc");
  delay(2000);
  Menu_T_WFQ_parametros();
}


//Para capturar los botnes del Keypad de la LCD
int read_LCD_buttons()   // função para ler os valores
{
  adc_key_in = analogRead(0);
  if (adc_key_in > 1000) {
    return btnNONE;
    //state = "Nada";
    key_stado = 5;
  }

  else if (adc_key_in < 50) {
    return btnRIGHT;
    //state = "RIGHT";
    key_stado = 0;
  }

  else  if (adc_key_in < 250) {
    return btnUP;
    //state = "UP";
    key_stado = 1;
  }

  else if (adc_key_in < 450) {
    return btnDOWN;
    //state = "DOWN";
    key_stado = 2;
  }

  else  if (adc_key_in < 650) {
    return btnLEFT;
    //state = "LEFT";
    key_stado = 3;
  }

  else if (adc_key_in < 850) {
    return btnSELECT;
    //state = "SELECT";
    key_stado = 4;
  }

  else {
    return btnNONE;  // when all others fail, return this...
    //state = "Ninguen B opr";
    //key_stado = 5;
  }
}



void Iniciar_ProtMensa() {
  int dIn = 500;

  lcd.setCursor(0, 0);
  lcd.print("  Prototipo V1");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando");
  delay(1000);
  lcd.print(".");
  delay(dIn);
  lcd.print(".");
  delay(dIn);
  lcd.print(".");
  delay(dIn);
  lcd.print(".");
  delay(dIn);
  lcd.print(".");
  delay(dIn);
  lcd.print(".");
  delay(dIn);
  delay(dIn);
  lcd.clear();
}


/*

  lcd.setCursor(0, 0);
  lcd.print("M I2C W_DAC");
  lcd_key = 0;
  lcd_key = read_LCD_buttons();  // Lê os botões
  delay(100);

  if (lcd_key == 0 ) {
    lcd.setCursor(0, 1);
    lcd.print("Canal 1");
    Wire.beginTransmission(1);
    Wire.write("Enviando canal 1");//Imprimimos cuando se realize la peticion ese mensaje y lo enviaremos por el canal 1
    Wire.endTransmission();
    Serial.println("Dato enviado al canal 1");//IMPRIMIMOS DE QUE HEMOS PEDIDO EL MESNAJE AL DISPOSITIVO EN EL CANAL 1
  } else   if (lcd_key == 3 ) {
    lcd.setCursor(0, 1);
    lcd.print("Canal 2");

    Wire.beginTransmission(2);
    Wire.write("enviando");
    Wire.write(" End_T");
    Wire.endTransmission();

    Serial.println("Dato enviado al canal 2");//IMPRIMIMOS DE QUE HEMOS PEDIDO EL MESNAJE AL DISPOSITIVO EN EL CANAL 1
  }

  delay(500);

  if (lcd_key == 0 ) {
    lcd.setCursor(0, 0);
    lcd.print("Canal 1");
    lcd.setCursor(0, 1);
    lcd.print(mensaje);
  } else   if (lcd_key == 3 ) {
    lcd.setCursor(0, 0);
    lcd.print("Canal 2");
    lcd.setCursor(0, 1);
    lcd.print(mensaje);
  } else {
    lcd.clear();
  }
*/
