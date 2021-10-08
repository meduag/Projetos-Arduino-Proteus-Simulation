// Manipulação de pinos para controle do Bluetooth
#define BLE_OUT         DDRA |= (1<<DDB0);              // A0 Configurado como saída
#define BLE_ON          PORTA |= (1<<PORTA0);           // A0 em 1 (manipula a porta entera) >> Liga o Bluetooth On HIGH
#define BLE_OFF         PORTA &= ~(1<<PORTA0);

#define Led_w1  9                                         // D9 Configurado como saída >> Data out LC
#define Led_w2  10

#define In_r 3       // Pin Lee tensao de entrada carregando
#define Bat_r 6      // Pin Lee tensao da bat
#define Btn_r  DDRB &= ~(1<<DDB2)

unsigned long tempo_atual = 0;
unsigned long tempo_final = 1000000;
unsigned long tt = 1000000;

char ID1[20] = "AT+NAMELed_Control  ";

int sal = 0;

boolean Led = false;

String time_led = "100";

String data = "-";

void setup() {
  BLE_OUT;
  BLE_ON;

  Btn_r;

  pinMode(Led_w1, OUTPUT);
  pinMode(Led_w2, OUTPUT);

  digitalWrite(Led_w1, Led);
  digitalWrite(Led_w2, Led);

  Serial.begin(9600); // Habilita comunicação
  ble_rename();       // Funçao que Renomea o Bt
}

void loop() {
  if (Serial.available()) {
    data = Serial.readStringUntil('<');
    Serial.read();

    if (data.charAt(0) == '1') {
      digitalWrite(Led_w1, HIGH);
      digitalWrite(Led_w2, HIGH);
    }//fin data 1

    if (data.charAt(0) == '2') {
      digitalWrite(Led_w1, LOW);
      digitalWrite(Led_w2, LOW);
    }//fin data 2

    if (data.charAt(0) == '3') {
      tempo_atual = micros();
      tempo_final = tt + tempo_final;
      Led = true;

      digitalWrite(Led_w1, Led);
      digitalWrite(Led_w2, !Led);

      sal = 1;

      while (sal == 1) {
        // controlo la salida del while con el boton
        if (PINB & (1 << PINB2)) {
          sal = 0;
        } else {
          sal = 1;
        }

        // controlo la salida del while con el serial
        if (Serial.available()) {
          data = Serial.readStringUntil('<');
          time_led = Serial.readStringUntil('<');
          if (data.charAt(0) == '4') {
            Serial.print("Time Led: ");
            Serial.println(time_led);
            tt = time_led.toInt();
          }
          if (data.charAt(0) == '5') {
            sal = 0;
          }

        }//fin serial detro del while

        /* aqui parpadeo el led */
        if (micros() > tempo_final) {
          Led = !Led;
          tempo_final = tempo_final + tt;
          Serial.print("Led: ");
          Serial.println(Led);
        }

        digitalWrite(Led_w1, Led);
        digitalWrite(Led_w2, !Led);

      }//fin while
      digitalWrite(Led_w1, LOW);
      digitalWrite(Led_w2, LOW);
    }//fin data 3

  }



  if (PINB & (1 << PINB2)) {
    Led = !Led;
    digitalWrite(Led_w1, Led);
    digitalWrite(Led_w2, !Led);
    Serial.print("Led: ");
    Serial.println(Led);
    delay(400);
    tt = 1000000;
  }//fin boton
}

/***********************************************************************************/
/***************    Função para renomear o modulo BT   *****************************/
/***********************************************************************************/
void ble_rename() {
  BLE_OFF;
  delay(100);
  BLE_ON;
  delay(100);

  Serial.write("AT+RESET"); // Reiniciar Módulo Bt
  Serial.println();
  delay(200);

  Serial.write("AT+SAVE0"); // Configura o módulo para que nao salve o último endereço conectado
  Serial.println();
  delay(200);

  Serial.write(ID1); // Renomea o Módulo Bt para E-Lastic
  Serial.println();
  delay(200);
}
