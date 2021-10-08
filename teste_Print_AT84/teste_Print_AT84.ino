char ID1[20] = "AT+NAMEVirus_Luz     ";

#define LED_PIN  A6
boolean s = HIGH;

// Manipulação de pinos para controle do Bluetooth
#define BLE_OUT         DDRA |= (1<<DDB0);              // A0 Configurado como saída
#define BLE_ON          PORTA |= (1<<PORTA0);           // A0 em 1 (manipula a porta entera) >> Liga o Bluetooth On HIGH
#define BLE_OFF         PORTA &= ~(1<<PORTA0);          // A0 em 0 (manipula a porta entera) >> Desliga o Bluetooths Off LOW

void setup() {
  // Configuraçoes iniciais
  BLE_OUT;            // Habilita o pin controle do Bt
  BLE_ON;             // Liga o Bth
  Serial.begin(9600);
  ble_rename();
  BLE_ON;             // Liga o Bth

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    Serial.write(c);
    if (c == 'c') {
      digitalWrite(LED_PIN, !s);
      delay(1000);
    }

  }
}

/***********************************************************************************/
/***************    Função para renomear o modulo BT   *****************************/
/***********************************************************************************/
void ble_rename() {
  BLE_OFF; // Deliga e liga o Bluetooth
  delay(100);
  BLE_ON;
  delay(100);

  Serial.write("AT+RESET"); // Reiniciar Módulo Bt
  Serial.println();
  delay(500);

  Serial.write("AT+SAVE0"); // Configura o módulo para que nao salve o último endereço conectado
  Serial.println();
  delay(500);

  Serial.write(ID1); // Renomea o Módulo Bt para E-Lastic
  Serial.println();
  delay(500);

  BLE_OFF;            // Desliga o Bt
  delay(100);
}
