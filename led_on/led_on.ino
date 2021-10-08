#define BAT_LED_OUT     DDRA |= (1<<DDA6);
#define BAT_LED_IN      DDRA &= ~(1<<DDA6);
#define BAT_LED_ON      PORTA &= ~(1<<PORTA6); // desliga o led 
#define BAT_LED_OFF     PORTA |= (1<<PORTA6); // desliga o led
#define BAT_LED_TOGGLE  PINA |= (1<<PINA6);

void setup() {
  BAT_LED_OUT;
  BAT_LED_OFF;

}

void loop() {
  // put your main code here, to run repeatedly:

}
