int Canal_1 = 2;
float Periodo=0;
float T1=0;
float T2=0;

void setup() {                
  // initialize the digital pin as an output.
  Serial.begin(9600);
  pinMode(Canal_1, OUTPUT);     
}

// the loop routine runs over and over again forever:
void loop() {
  //Valores da Funçao= 1-Frequancia, 2-Porcentagem T1, 3- Tempo Morto do sinal
  CriarOndaQuadrada(30, 50, 0, 0);
}



void CriarOndaQuadrada(double Freq, float PorT_Alto, float T_Inter, float T_Dead) {
  
  Periodo = 1 / Freq;
  T1 = (Periodo * (PorT_Alto/100))*1000;
  T2 = 100 - PorT_Alto;
  T2 = (Periodo * (T2/100))*1000;
  
  Serial.print("Valor T1: ");
  Serial.println(T1);
  
  //T1 = (int)T1;
  
  Serial.print("Valor T2: ");
  Serial.println(T2);
  
  //T2 = (int)T2;
  
  //funciona con milisegundos hasta 300 Herz
  //tiene que usar microsegundos a partir de 301 Hz
  
  digitalWrite(Canal_1, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(T1);               // wait for a second
  
  if(T_Inter > 0){
   delay(T_Dead);
  }
  
  digitalWrite(Canal_1, LOW);    // turn the LED off by making the voltage LOW
  delay(T2);
  
  if(T_Dead > 0){
   delay(T_Dead);
  }
  
  
}
