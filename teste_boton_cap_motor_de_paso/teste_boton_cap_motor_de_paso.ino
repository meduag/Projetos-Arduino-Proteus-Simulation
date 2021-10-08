// Pin Enable
int ENA = 5;

// Pin Direccion
int DIR = 2;

// Pin Pulso
int PULSO = 3;

// Led indicador
int LED = 13;

// Division de pasos para la rampa
int dv = 400;

// Tiempo entre micro pasos de aceleracion constante
int mp = 900;

// Tiempo entre micro pasos de aceleracion de la rampa subida y bajada
int ti = 2000;

// Valor del boton de entrada
int valA0 = 0;

// Para controlar las acciones del boton
bool act = false, dirm = false;


void setup() {
  Serial.begin(115200);

  pinMode(A0, INPUT);

  pinMode(ENA, OUTPUT);

  pinMode(DIR, OUTPUT);
  pinMode(PULSO, OUTPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(DIR, LOW);
  digitalWrite(PULSO, LOW);
  digitalWrite(LED, LOW);

}

void loop() {

  valA0 = analogRead(A0);
  if (valA0 > 500) {
    while (valA0 > 500) {
      digitalWrite(LED, HIGH);
      delay(70);
      digitalWrite(LED, LOW);
      delay(70);
      Serial.println("Boton presionado");
      valA0 = analogRead(A0);
    }
    act = !act;
    Serial.println("Boton liberado");
  }

  if (act == true) {
    Serial.println("Moviendo motor");
    digitalWrite(ENA, LOW);
    dirm = !dirm;
    Serial.println("Dir motor");
    Serial.println(dirm);
    digitalWrite(DIR, dirm);
    digitalWrite(LED, HIGH);

    Serial.println("Ramp Up");
    ramp_up();

    Serial.println("Constante");
    for (int x = 0; x < 1000; x++) {
      digitalWrite(PULSO, HIGH);
      delayMicroseconds(mp);
      digitalWrite(PULSO, LOW);
      delayMicroseconds(mp);
    }

    Serial.println("Ramp Down");
    ramp_dw();

    digitalWrite(LED, LOW);
    digitalWrite(ENA, HIGH);
    Serial.println("Deshabilitando motor");
    act = false;
  } else {
    digitalWrite(ENA, HIGH);
    digitalWrite(DIR, LOW);
    digitalWrite(LED, LOW);
  }
}

void ramp_up() {
  // int dvi = mp / dv;
  int dvf = (ti - mp ) / dv;
  int dv1 = 0;

  int v_up = ti;
  //int v_dw = mp;

  for (int x = 0; x <= dv; x++) {

    Serial.print("#");
    Serial.print(x);
    Serial.print("\t Up: ");
    Serial.println(v_up);

    digitalWrite(PULSO, HIGH);
    delayMicroseconds(v_up);
    digitalWrite(PULSO, LOW);
    delayMicroseconds(v_up);

    dv1 += dvf;
    //v_dw = mp + dv1;
    v_up = ti - dv1;


  }
}

void ramp_dw() {
  int dvf = (ti - mp ) / dv;
  int dv1 = 0;

  int v_dw = mp;

  for (int x = 0; x <= dv; x++) {

    Serial.print("#");
    Serial.print(x);
    Serial.print("\t Up: ");
    Serial.println(v_dw);

    digitalWrite(PULSO, HIGH);
    delayMicroseconds(v_dw);
    digitalWrite(PULSO, LOW);
    delayMicroseconds(v_dw);

    dv1 += dvf;
    v_dw = mp + dv1;
  }
}
