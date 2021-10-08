#include <LiquidCrystal.h>  //Inclui a biblioteca para operar o display LCD
#include <Servo.h>          //Inclui a biblioteca para operar motores servo

#define botao 8             //Botao ligado no pino digital 8
#define potenciometro A0    // Potenciometro ligado no pino analogico 0
#define MAX_TAM_SENHA 4    //Tamanho da senha
#define SENHA_CURTA   0
#define SENHA_ERRADA  -1
#define SENHA_CORRETA 1

// Mudar essas variaveis se precisar que o servo pare em outras posicoes
// para nao forcar dependendo de como esta construida a fechadura
#define SERVO_FECHADURA_ABERTA 35 // NO MAXIMO 180
#define SERVO_FECHADURA_FECHADA 0 // NO MINIMO 0

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //cria um objeto display LCD nestes pinos
Servo fechadura;  // cria um objeto servo que acionara a fechadura

int map_pot; //Variavel para armazenar o valor da escala de 0 a 5 do potenciometro
int posicaoSenha = 0; //Variavel para contagem de digitos da senha, comeca em zero
int estadoBotao = LOW; // variavel para registrar o estado do botao (HIGH se apertado, LOW se solto)


int senha[] = {0, 0, 0, 0};           //Variavel para armazernar os digitos da senha sendo digitada
int senhadesejada[] = {0, 2, 5, 1};   //Variavel para armazenar a senha correta

void setup()
{

  lcd.begin(16, 2);     //Inicia o display LCD que no nosso caso eh de 16 colunas por 2 linhas
  fechadura.attach(9);  // associa o servo da fechadura ao pino 9 do Arduino
  //Serial.begin(9600); // abre a porta serial em 9600 bps (para testes)

  pinMode(A1, OUTPUT); //Configura o pino A1 como saida para LED 1
  pinMode(A2, OUTPUT); //Configura o pino A2 como saida para LED 2
  pinMode(A3, OUTPUT); //Configura o pino A3 como saida para LED 3
  pinMode(A4, OUTPUT); //Configura o pino A4 como saida para LED 4
  pinMode(A5, OUTPUT); //Configura o pino A5 como saida para LED 5

  pinMode(botao, INPUT); //Configura o pino do botao como entrada

  lcd.clear();  //Limpa a tela do LCD
  resetSenha(); //Zera a senha para comecar
  //Serial.print ("posicoes: "); Serial.print(posicaoSenha+1); Serial.print(" do max de "); Serial.print(MAX_TAM_SENHA); Serial.print(" (INICIAL)\n");

  // Faz um movimento de abre e fecha ao iniciar da primeira vez (ou sempre que faltar energia no Arduino)
  // Pode comentar as 3 proximas linhas para eliminar esse comportamento
  //
  abreFechadura();
  delay(500);
  fechaFechadura();

}


// ===========================
// LOOP PRINCIPAL DA FECHADURA
// ===========================

void loop()
{
  int statusSenha = SENHA_CURTA;

  map_pot = lePotenciometro(5); //Le posicao do potenciometro, em escala de 0 a 5

  //Serial.print(map_pot); Serial.print("\n");

  // MOSTRAR A POSICAO DO POTENCIOMETRO E A SENHA SENDO DIGITADA
  lcd.setCursor(posicaoSenha, 0); //cursor na primeira linha do display, coluna correspondente ao digito corrente sendo entrado
  lcd.print(map_pot);             //mostra digito que esta sendo registrado no potenciometro (igual ao que os LEDs estao mostrando)
  acendeLEDs(map_pot);            //mostra digito que esta sendo registrado no potenciometro (igual ao que o display esta mostrando)

  // CHECAR SE O BOTAO FOI APERTADO
  estadoBotao = checaBotao();

  //Serial.print(estadoBotao); Serial.print("\n");

  // SE O BOTAO FOR APERTADO, DECIDE O QUE FAZER
  if (estadoBotao == HIGH)            // HIGH = botao foi apertado
  {
    carregaDigitoNaSenha(map_pot);  // sempre que apertar o botao, carrega mais um digito na senha

    statusSenha = checaSenha();     // da uma olhada se a senha ja esta boa
    if (statusSenha != SENHA_CURTA) // senha curta = ainda nao terminou de entrar todos os digitos, nao faz nada por enquanto
      if (statusSenha == SENHA_CORRETA) trataSenhaCerta(); // senha completa e esta correta -- pode abrir
      else trataSenhaErrada();    // senha completa mas esta incorreta -- avisa do erro e prepara para recomecar
  }
}
// ===========================
// ===========================




// ====================================
// FUNCOES CHAMADAS PELO LOOP PRINCIPAL
// ====================================



// Acende LEDs de acordo com o numero lido do potenciometro
// ========================================================
void acendeLEDs(int numeroDeLEDs)
{
  if (numeroDeLEDs > 0 )
    digitalWrite(A1, HIGH); // Se numero desejado for maior que zero, acende o LED 1
  else digitalWrite(A1, LOW); //Senao apaga o LED 1
  if (numeroDeLEDs > 1 ) digitalWrite(A2, HIGH); //Se numero desejado for maior que 1 acende tambem o LED 2
  else digitalWrite(A2, LOW); //Senao apaga o LED 2
  if (numeroDeLEDs > 2 ) digitalWrite(A3, HIGH); //Se numero desejado for maior que 2 acende tambem o LED 3
  else digitalWrite(A3, LOW); //Senao apaga o LED 3
  if (numeroDeLEDs > 3 ) digitalWrite(A4, HIGH); //Se numero desejado for maior que 3 acende tambem o LED 4
  else digitalWrite(A4, LOW); //Senao apaga o LED 4
  if (numeroDeLEDs > 4 ) digitalWrite(A5, HIGH); //Se numero desejado for maior que 4 acende por fim o LED 5
  else digitalWrite(A5, LOW); //Senao apaga o LED 5
}

// ZERA A SENHA E PREPARA TUDO PARA DIGITAR NOVAMENTE
// ========================================================
void resetSenha(void)
{
  lcd.clear();
  lcd.setCursor(0, 0); //cursor no inicio da primeira linha do display
  lcd.print("____  <-- senha");
  posicaoSenha = 0; // Retorna para primeira posicao da senha
  lcd.setCursor(0, 1); //cursor no inicio da segunda linha do display
  lcd.print("        ");
  lcd.setCursor(0, 0); //cursor no inicio da primeira linha do display
}

// CARREGA NA SENHA O DIGITO QUE ESTA SELECIONADO NO POTENCIOMETRO NO MOMENTO
// ==========================================================================
void carregaDigitoNaSenha(int digito)
{
  // Nao faz nada se o tamanho maximo da senha tiver sido excedido
  if (posicaoSenha > MAX_TAM_SENHA - 1) return;

  senha[posicaoSenha] = digito; // coloca o digito escolhido na posicao certa da senha

  lcd.setCursor(posicaoSenha, 0); //Posiciona o cursor na coluna correpondente a posicao na senha

  // Mostra rapidamente o digito entrado
  lcd.print(digito); //Imprime o digito
  delay(200);

  // E em seguida oculta o numero com um asterisco
  lcd.setCursor(posicaoSenha, 0); //Posiciona o cursor na coluna correpondente a posicao na senha
  lcd.print("*"); //Imprime um asterisco na mesma posicao, para ocultar o digito

  // agora incrementa a posicao da senha
  posicaoSenha++;
}

// CHECA SE A SENHA ESTA CURTA/INCOMPLETA, OU COMPLETA CORRETA OU COMPLETA INCORRETA
// =================================================================================
int checaSenha(void)
{
  if (posicaoSenha < MAX_TAM_SENHA)
  {
    //Serial.print ("posicoes: "); Serial.print(posicaoSenha+1); Serial.print(" do max de "); Serial.print(MAX_TAM_SENHA); Serial.print(" -- senha curta");
    //Serial.print("-- Senha = ["); Serial.print(senha[0]); Serial.print(senha[1]); Serial.print(senha[2]); Serial.print(senha[3]); Serial.print("]\n");
    return SENHA_CURTA;
  }
  else
  {
    //Serial.print ("senha completa (numero correto de digitos entrados\n");

    if (senha[0] == senhadesejada[0] && senha[1] == senhadesejada[1] && senha[2] == senhadesejada[2] && senha[3] == senhadesejada[3])
    {
      //Serial.print("Senha correta");
      //Serial.print("-- Senha = ["); Serial.print(senha[0]); Serial.print(senha[1]); Serial.print(senha[2]); Serial.print(senha[3]); Serial.print("]\n");
      return SENHA_CORRETA;
    }
    else
    {
      //Serial.print("Senha errada");
      //Serial.print("-- Senha = ["); Serial.print(senha[0]); Serial.print(senha[1]); Serial.print(senha[2]); Serial.print(senha[3]); Serial.print("]\n");
      return SENHA_ERRADA;
    }
  }
}

// ACOES A TOMAR SE A SENHA ESTIVER CORRETA
// ========================================================
void trataSenhaCerta(void)
{
  lcd.clear();
  lcd.setCursor(0, 0); //cursor no inicio da primeira linha do display
  lcd.print("FECHADURA ABERTA");

  delay(200);

  // codigo para abrir a fechadura aqui:
  abreFechadura();

  lcd.setCursor(0, 1); //cursor no inicio da segunda linha do display
  lcd.print("Aperte p trancar");

  // animacao nos LEDs acendendo de 0 a 5
  for (int i = 0; i < 6; i++) {
    acendeLEDs(i);
    delay(150);
  }

  esperaBotao(0); // aguarda que aperte o botao por tempo indeterminado

  // codigo para trancar a fechadura aqui:
  fechaFechadura();

  // animacao nos LEDs apagando de 5 a 0
  for (int i = 5; i > -1; i--) {
    acendeLEDs(i);
    delay(150);
  }
  resetSenha(); // prepara tudo para comecar nova tentativa de entrar a senha
  delay(200);   // espera um pouquinho para tudo se acalmar

}

// ACOES A TOMAR SE A SENHA ESTIVER INCORRETA
// ========================================================
void trataSenhaErrada(void)
{
  acendeLEDs(0); // apaga todos os LEDs
  lcd.setCursor(0, 0); //cursor no inicio da primeira linha do display
  lcd.print("Senha incorreta.");

  lcd.setCursor(0, 1); //cursor no inicio da segunda linha do display

  // animacao nos LEDs para senha errada (piscar todos os leds juntos algumas vezes)
  for (int i = 0; i < 6; i++) {
    acendeLEDs(5);
    delay(100);
    acendeLEDs(0);
    delay(100);
  }
  lcd.print("Aperte o botao...");

  esperaBotao(3000); // aguarda que aperte o botao ou que passem 3 segundos
  resetSenha();      // prepara tudo para comecar nova tentativa de entrar a senha
  delay(200);        // espera um pouco para tudo se acalmar
}


// FUNCAO PARA ESPERAR O BOTAO SER APERTADO, OU QUE PASSE ALGUM TEMPO (O QUE ACONTECER PRIMEIRO)
// SE RECEBER tempo = 0 COMO PARAMETRO, ESPERA O BOTAO SER APERTADO POR TEMPO INDETERMINADO
// =============================================================================================
void esperaBotao(int tempo)
{
  int tictoc = 0;
  if (tempo)
  {
    do
    {
      delay(1); // espera um milissegundo
      tictoc++; // incrementa o contador de tempo
    }
    while (checaBotao() == LOW && tictoc < tempo); //espera aqui ate que botao seja apertado ou que passe o numero de milissegundos na variavel tempo
  }
  else while (checaBotao() == LOW) {}; //espera o botao ser apertado por tempo indeterminado
}


// LE A POSICAO DO POTENCIOMETRO E MAPEIA PARA NUMERO DE LEDS QUE ESTAMOS USANDO
// =============================================================================
int lePotenciometro(int maxPot)
{
  int temp = 0;

  temp = map(analogRead(potenciometro), 0, 1023, 0, maxPot + 1); //Retorna a posicao do potenciometro mapeado para escala de 0 a maxPot
  if (temp > maxPot - 1) temp = maxPot;
  return temp;
}



// CHECA SE O BOTAO ESTA APERTADO OU NAO
// =====================================
int checaBotao(void)
{
  int buttonState = LOW;
  buttonState = digitalRead(botao); // ler estado do botao
  return buttonState;
}


// ACOES A TOMAR PARA ABRIR A FECHADURA
// =====================================
void abreFechadura(void)
{
  fechadura.write(SERVO_FECHADURA_FECHADA);
}


// ACOES A TOMAR PARA TRANCAR A FECHADURA
// =====================================
void fechaFechadura(void)
{
  fechadura.write(SERVO_FECHADURA_ABERTA);
}
