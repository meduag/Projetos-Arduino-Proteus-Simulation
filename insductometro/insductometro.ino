#include <Arduino.h>

//13 é a entrada do circuito (conecta-se ao resistor de 150 ohm), 11 é a saída do comparador/amplificador operacional.
double pulse, frequency, capacitance, inductance;

void setup() {
  Serial.begin(9600);
  pinMode(11, INPUT);
  pinMode(13, OUTPUT);
  Serial.println("Iniciando Medição...");
  delay(1000);
}
void loop() {
  
  digitalWrite(13, HIGH);
  delay(5);//dê algum tempo para carregar o indutor.
  digitalWrite(13, LOW);
  
  delayMicroseconds(100); //verifique se a ressonância é medida.
  
  pulse = pulseIn(11, HIGH, 5000); //retorna 0 se o limite de tempo chegar
  
  Serial.println(pulse);
  
  if (pulse > 0.1) { //se o tempo limite não ocorrer e dessa forma proceder a leitura:
    
    //#error insira o valor de capacitância utilizada aqui. Atualmente estamos utilizando 2uF. Exclua esta linha depois de inserir sua própria capacitância!
    capacitance = 2E-6; // - insira o valor da capacitância utilizada aqui. Atualmente setamos 2uF!

    frequency  = 1.E6 / (2 * pulse);
    inductance = 1. / (capacitance * frequency * frequency * 4.*3.14159 * 3.14159);
    inductance *= 1E6;
    //double ind_mH = 

    //Impressão Serial (Monitor Serial da IDE)
    Serial.print("uS: ");
    Serial.print( pulse );
    Serial.print("\t Hz: ");
    Serial.print( frequency );
    Serial.print("\t uH: ");
    Serial.print( inductance );
    inductance = inductance / 1000;
    Serial.print("\t mH: ");
    Serial.println( inductance );
    delay(1000);
  }
}
