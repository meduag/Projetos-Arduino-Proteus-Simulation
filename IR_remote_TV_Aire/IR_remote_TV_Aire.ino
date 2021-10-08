#include <IRremote.h>

IRsend irsend;  // Un Led IR para controlar en el pin 8 en el atmega8

const int boton = 8;

void setup() {
  //irsend.begin();
  pinMode(boton, INPUT);
}

void loop() {
  int reading = digitalRead(boton);
  if (reading  == HIGH ){
    irsend.sendSony(0xA90, 12);
  }
  delay(100);
}
