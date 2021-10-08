#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

#define SCREEN_WIDTH 128                // OLED display width
#define SCREEN_HEIGHT 64                // OLED display height

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1      // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String message; //string that stores the incoming message
int pasos = 100;
char outstr[15];

const int step_M = 8;
const int dir_M = 9;
const int ene_M = 10;
int t_ep = 500;
int m = 0;  // 0 es para ir y venir, 1 solo para ir frente, 2 para ir atras;

int comp = 0, compb = 0;
bool dir_v = true;

int Distance = 0; // Record the number of steps we've taken void setup() {

void startScreen();
void printLCD();
void pasos_motor(int ps);
void separar(String msn);
void printLCD_fin();

SoftwareSerial mySerial(2, 3); // RX, TX

void setup() {
  Serial.begin(115200);
  //Serial.setTimeout(1);
  mySerial.begin(9600);
  //Serial1.setTimeout(1);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 failed"));
    for (;;);                               // Don't proceed, loop forever
  }

  startScreen();

  pinMode(step_M, OUTPUT);
  pinMode(dir_M, OUTPUT);
  pinMode(ene_M, OUTPUT);
  digitalWrite(step_M, LOW);
  digitalWrite(dir_M, LOW);
  digitalWrite(ene_M, LOW);
}

void loop() {

  while (Serial.available()) { //while there is data available on the serial monitor
    message += char(Serial.read()); //store string from serial command
    comp++;
  }

  if (comp > 0 && message != "") {
    Serial.println("Serial PC <" + message); //show the data
    mySerial.print(message); //show the data
    separar(message);
    comp = 0;
  }

  if (!Serial.available()) { //This comes from the console
    if (message != "") { //if data is available
      Serial.println("PC <" + message); //show the data
      mySerial.print(message); //show the data
      message = ""; //clear the data
    }
  }

  while (mySerial.available()) { //while there is data available on the serial monitor
    //if (Serial1.available() != 0) {
    message += char(mySerial.read()); //store string from serial command
    //message = Serial.readStringUntil('<');

    compb = 1;
  }

  if (compb > 0 && message != "") {
    Serial.println("Serial Bt <" + message); //show the data
    mySerial.println(message); //show the data
    mySerial.println("Separando"); //show the data
    separar(message);
    compb = 0;
  }

  if (!mySerial.available()) { //This comes from the bluetooth device (such as phone)
    if (message != "") { //if data is available
      Serial.println("Bt >" + message); //show the data
      mySerial.println(message); //show the data
    }
  }
  delay(500); //delay
}

void pasos_motor(int ps) {
  int cv = 0;
  Distance = 0;
  switch (m) {
    case 0:
      Serial.println("Entro en el While M0");
      cv = 0;
      digitalWrite(ene_M, HIGH);
      while (cv <= 3) {
        digitalWrite(step_M, HIGH);
        delayMicroseconds(100);
        digitalWrite(step_M, LOW);
        delayMicroseconds(100);
        Serial.println(Distance);
        Distance++;

        if (Distance == ps) { // We are! Reverse direction (invert DIR signal)
          delay(t_ep);
          /*if (digitalRead(dir_M) == LOW) {
            digitalWrite(dir_M, HIGH);
            } else {
            digitalWrite(dir_M, LOW);
            } // Reset our distance back to zero since we're // starting a new move
          */
          dir_v = !dir_v;
          digitalWrite(dir_M, dir_v);
          Distance = 0; // Now pause for half a second delay(500);
          cv++;
        }
      }
      digitalWrite(ene_M, LOW);
      break;
    case 1:
      cv = 0;
      Serial.println("Entro en el While M1");
      digitalWrite(dir_M, LOW);
      digitalWrite(ene_M, HIGH);
      while (cv <= 1) {
        digitalWrite(step_M, HIGH);
        delayMicroseconds(100);
        digitalWrite(step_M, LOW);
        delayMicroseconds(100);
        Serial.println(Distance);
        Distance++;

        if (Distance >= ps) { // We are! Reverse direction (invert DIR signal)
          cv++;
        }
      }
      digitalWrite(ene_M, LOW);
      break;
    case 2:
      cv = 0;
      Serial.println("Entro en el While M2");
      digitalWrite(dir_M, HIGH);
      digitalWrite(ene_M, HIGH);
      while (cv <= 1) {
        digitalWrite(step_M, HIGH);
        delayMicroseconds(100);
        digitalWrite(step_M, LOW);
        delayMicroseconds(100);
        Serial.println(Distance);

        Distance++;

        if (Distance >= ps) { // We are! Reverse direction (invert DIR signal)
          cv++;
        }
      }
      digitalWrite(ene_M, LOW);
      break;
  }
  Serial.println("Fin");
  printLCD_fin();
}

void separar(String msn) {
  String msns = "";
  int num = msn.length();
  Serial.print("numero de caracteres: ");
  Serial.println(num);
  int c = 0;
  Serial.print("Primer caracter: ");
  Serial.println(msn.charAt(c));
  //delay(5000);

  if (msn.charAt(c) == 't') { // compruebo si es tiempo entre motores
    c = 1;
    while (c <= num + 1) {
      msns += msn.charAt(c);
      c++;
    }
    t_ep = msns.toInt(); //actualizo tiempo entre pasos
    Serial.println("actualizo tiempo entre pasos");
    Serial.print("Tempo entre pasos: ");
    Serial.println(t_ep);

    msns = "";
  }

  if (msn.charAt(c) == 'p') { // compruebo si es tiempo entre motores
    c = 1;
    while (c <= num + 1) {
      msns += msn.charAt(c);
      c++;
    }
    pasos = msns.toInt(); // actualizo cantidad de pasos
    Serial.println("actualizo cantidad de pasos");
    Serial.print("Cantidad de pasos: ");
    Serial.println(pasos);
    msns = "";
  }

  if (msn.charAt(c) == 'm') { // compruebo el tipo de paso
    c = 1;
    if (msn.charAt(c) == '0') { // va y viene
      m = 0;
      Serial.println("Tipo de teste: va y viene");
    }

    if (msn.charAt(c) == '1') { // solo va
      m = 1;
      Serial.println("Tipo de teste: solo va");
    }

    if (msn.charAt(c) == '2') { // solo viene
      m = 2;
      Serial.println("Tipo de teste: solo viene");
    }
  }

  c = 0;
  msns = "";
  printLCD();
  pasos_motor(pasos);
}

void startScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(1, 1);
  display.println("Control Motor de paso");
  display.setCursor(1, 15);
  display.println("LED e Bluetooth");
  display.display();
  delay(1000);
}

void printLCD() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(1, 1);
  display.println("Control Motor de paso");
  // actualiza modo
  display.setCursor(1, 15);
  display.print("Modo: ");
  display.setCursor(30, 15);
  dtostrf(m, 10, 1, outstr);
  display.println(outstr);
  //actualiza pasos
  display.setCursor(1, 30);
  display.print("Paso: ");
  display.setCursor(30, 30);
  dtostrf(pasos, 10, 1, outstr);
  display.println(outstr);
  //actualiza tiempo
  display.setCursor(1, 45);
  display.print("T_ep: ");
  display.setCursor(30, 45);
  dtostrf(t_ep, 10, 1, outstr);
  display.println(outstr);

  display.display();
  //delay(1000);
}

void printLCD_fin() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(1, 1);
  display.println("Termino");

  display.display();
  delay(500);
}
