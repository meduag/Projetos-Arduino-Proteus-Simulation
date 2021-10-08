#include <Wire.h>
#include <SPI.h>
#include <Adafruit_SSD1306.h>

#define ledPin 13 // numero do pino onde o LED esta conectado

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

char received_data; // armazena o dado recebidos
volatile boolean received_flag; // flag de recebimento de dado

int i = 0;


void setup() {
  // inicia a SPI no modo escravo
  SPCR |= bit (SPE);
  
  // configura o pino MISO como saida
  pinMode(MISO, OUTPUT);
  
  // prepara o flag de recebimento para interrupcao
  received_flag = false;
  
  // liga as interrupcoes
  SPI.attachInterrupt();

  pinMode(ledPin, OUTPUT);  // configura o pino do LED como saida
}

void loop() {
  // se o flag de recebimento for verdadeiro:
  if (received_flag){
    // se o byte recebido for igual a 0, apaga o LED
    if (received_data == 0) {
      digitalWrite(ledPin, LOW);
    }

    // se o byte recebido for igual a 1 acende o LED
    if (received_data == 1) {
      digitalWrite(ledPin, HIGH);
    }

    // limpa o flag de recebimento
    received_flag = false;
  }
}

// Rotina de interrupcao do SPI
ISR (SPI_STC_vect) {
  // le e salva o byte do Registrador de dados da SPI
  received_data = SPDR; 

  // seta o flag de recebimento para que o dado recebido 
  // seja processado no proximo loop
  received_flag = true;
}
