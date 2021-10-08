/* User configuration */
#define STD_DUTY_CYCLE 0.5
#define NEG_PW 500 // us
#define POS_PW 500 // us
#define INTERPHASE_DELAY 25 // us (offset de 30us)
#define INTERPULSE_DELAY 1000 // us
#define PWM_MODE 0 // 0: rectangular; 1: exponential
#define PWM_RANGE 256
#define NEG_TIME_OFFSET 50 // us
#define POS_TIME_OFFSET 50 // us

/* Initialization */
#define NEG_PIN 2
#define POS_PIN 4
#define PWM_PIN 3
#define PWM_PERIOD 0.00005
#define LED_PIN 13

void setup()
{
  TCCR2B = TCCR2B & B11111000 | B00000001; // set timer 2 divisor to 1 for PWM frequency of 31372.55 Hz
  pinMode(PWM_PIN, OUTPUT);
  pinMode(NEG_PIN, OUTPUT);
  pinMode(POS_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
//  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
//  attachInterrupt(INTERRUPT_PIN, stopPWM, FALLING);

  for (int i = 0; i < 5; i++)
  {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }

  analogWrite(PWM_PIN, 128);
}

void loop()
{
  digitalWrite(NEG_PIN, HIGH);
  delayMicroseconds(500);
  digitalWrite(NEG_PIN, LOW);
  delayMicroseconds(1500);
}
