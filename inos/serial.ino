#include <SoftwareSerial.h>

SoftwareSerial mySerial(0, 1); // RX, TX

int A = 2;
int B = 3;
int C = 4;
int D = 5;


void setup() {
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);

  Serial.begin(9600);
  //Serial.println("hola");
}


void loop() {
  for (int i = 1; i <= 13; i++) {
    selecPic(i);
    Serial.println("valor : ");
    Serial.println(i);
    Serial.println("\n");
    delay(500);
  }
}

void selecPic(int var) {
  switch (var) {
    case 1:
      ad_Pic(0, 0, 0, 0);
      break;
    case 2:
      ad_Pic(0, 0, 0, 1);
      break;
    case 3:
      ad_Pic(0, 0, 1, 0);
      break;
    case 4:
      ad_Pic(0, 0, 1, 1);
      break;
    case 5:
      ad_Pic(0, 1, 0, 0);
      break;
    case 6:
      ad_Pic(0, 1, 0, 1);
      break;
    case 7:
      ad_Pic(0, 1, 1, 0);
      break;
    case 8:
      ad_Pic(0, 1, 1, 1);
      break;
    case 9:
      ad_Pic(1, 0, 0, 0);
      break;
    case 10:
      ad_Pic(1, 0, 0, 1);
      break;
    case 11:
      ad_Pic(1, 0, 1, 0);
      break;
    case 12:
      ad_Pic(1, 0, 1, 1);
      break;
    default:
      ad_Pic(1, 1, 1, 1);
      break;
  }
}

void ad_Pic(int Dd, int Cc, int Bb, int Aa) {
  if (Aa == 1) {
    digitalWrite(A, HIGH);
  }
  else {
    digitalWrite(A, LOW);
  }

  if (Bb == 1) {
    digitalWrite(B, HIGH);
  }
  else {
    digitalWrite(B, LOW);
  }

  if (Cc == 1) {
    digitalWrite(C, HIGH);
  }
  else {
    digitalWrite(C, LOW);
  }

  if (Dd == 1) {
    digitalWrite(D, HIGH);
  }
  else {
    digitalWrite(D, LOW);
  }
}

