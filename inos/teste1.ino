// Projeto 20 – Arduino como um gerador de frequência (Onda quadrada)
  /*
Para sintaxe verificar o site
http://arduino.cc/en/Reference/Tone
  */

void setup() {
  
  Serial.begin(9600);  // Inicia o Serial Monitor
}

void loop() {
  Serial.println("A frequencia eh de: 0 Hz");  // Imprimi na tela a frequência gerada
  {
    tone(6, 0);  // Defini o pino em OUTPUT, e a frequencia gerada no pino
    delay(2000);  // Gera a frequência por 2 segundos 
  }
  Serial.println("A frequencia eh de: 100 Hz");
  {
    tone(6, 100); delay(2000);
  }
  Serial.println("A frequencia eh de: 200 Hz");
  {
    tone(6, 200); delay(2000);
  }
  Serial.println("A frequencia eh de: 300 Hz");
  {
    tone(6,300); delay(2000);
  }
  Serial.println("A frequencia eh de: 400 Hz");
  {
    tone(6,400); delay(2000);
  }
  Serial.println("A frequencia eh de: 500 Hz");
  {
    tone(6,500); delay(2000);
  }
  Serial.println("A frequencia eh de: 600 Hz");
  {
    tone(6,600); delay(2000);
  }
  Serial.println("A frequencia eh de: 700 Hz");
  {
    tone(6,700); delay(2000);
  }
  Serial.println("A frequencia eh de: 800 Hz");
  {
    tone(6,800); delay(2000);
  }
  Serial.println("A frequencia eh de: 900 Hz");
  {
    tone(6,900); delay(2000);
  }
  Serial.println("A frequencia eh de: 980 Hz");
  {
    tone(6,980); delay(2000);
  }
}
