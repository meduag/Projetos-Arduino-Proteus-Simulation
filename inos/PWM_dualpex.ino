/* User configuration */
#define STD_DUTY_CYCLE 0.9
#define NEG_PW 700 // us
#define POS_PW 500 // us
#define INTERPHASE_DELAY 0 // us (offset de 30us)
#define INTERPULSE_DELAY 2000 // us
#define PWM_MODE 0 // 0: rectangular; 1: exponential; 2: sinusoidal
#define PWM_RANGE 256
#define NEG_TIME_OFFSET 50 // us
#define POS_TIME_OFFSET 50 // us

/* Initialization */
#define NEG_PIN 2
#define POS_PIN 4
#define PWM_PIN 3
#define PWM_PERIOD 33
#define LED_PIN 13

/* Structures declaration */
enum pulse_status_t
{
  before_first_pulse,
  first_pulse,
  inter_phase,
  second_pulse,
  inter_pulse,
  none
} old_status, curr_status;

int t_this_status, t_this_PWM, t_now;
int dc;
int stopLoop;

/* Functions declaration */
int getDutyCycle(int dt, int mode);

void setup()
{  
  /* Variables initialization */
  dc = 0;
  t_this_status = 0;
  t_this_PWM = 0;
  t_now = 0;
  old_status = none;
  curr_status = inter_pulse;
  stopLoop = 0;

  /* System initialization */
  TCCR2B = TCCR2B & B11111000 | B00000001; // set timer 2 divisor to 1 for PWM frequency of 31372.55 Hz
  pinMode(PWM_PIN, OUTPUT);
  pinMode(NEG_PIN, OUTPUT);
  pinMode(POS_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

//  For debug
  for (int i = 0; i < 5; i++)
  {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
}

void loop()
{
  if (old_status != curr_status)
  {
    t_this_status = micros();
    old_status = curr_status;
  }
    
  /* A little before first pulse */
  if (curr_status == before_first_pulse)
  {
    if (PWM_MODE != 2)
      analogWrite(PWM_PIN, (int)(STD_DUTY_CYCLE*PWM_RANGE));
    else
      analogWrite(PWM_PIN, 255);
      
    delayMicroseconds(NEG_TIME_OFFSET);
  
    curr_status = first_pulse;
  }
    
  /* First pulse */
  else if (curr_status == first_pulse)
  {
    digitalWrite(NEG_PIN, HIGH);
    
    t_now = micros();
    while ((t_now - t_this_status) < NEG_PW)
    {
      dc = getDutyCycle((t_now - t_this_status), PWM_MODE);
      if (dc < 0)
        dc = 255-dc;
      analogWrite(PWM_PIN, dc);

      delayMicroseconds(10);
      t_now = micros();
    }
    
    digitalWrite(NEG_PIN, LOW); 
      
    curr_status = inter_pulse;
  }
    
  /* Interphase */
  else if (curr_status == inter_phase)
  {
    delayMicroseconds(INTERPHASE_DELAY);
    
    curr_status = second_pulse;
  }
    
  /* Second pulse */
  else if (curr_status == second_pulse)
  {
    digitalWrite(POS_PIN, HIGH);
    t_this_PWM = 0;
    t_now = micros();
    while ((t_now - t_this_status) < POS_PW)
    {
      if ((t_now - t_this_PWM) > PWM_PERIOD)
      {
        dc = getDutyCycle((t_now - t_this_status), PWM_MODE);
        
        t_this_PWM = micros();
      }
      
      t_now = micros();
    }
    digitalWrite(POS_PIN, LOW); 
    
    curr_status = inter_pulse;
  }
    
  /* Interpulse */
  else if (curr_status == inter_pulse)
  {
    if (PWM_MODE != 2)
      analogWrite(PWM_PIN, (int)(STD_DUTY_CYCLE*PWM_RANGE));
    else
      analogWrite(PWM_PIN, 255);
    delayMicroseconds(INTERPULSE_DELAY);
    
    curr_status = before_first_pulse;
  }
}

#define RC_CANCEL_FUNC (PWM_RANGE * (1-exp(-.0001*dt)))
#define RC_EXP_FUNC (PWM_RANGE * (exp(-.0025*dt)))
#define RC_SIN_FUNC_UP (PWM_RANGE * (sin(2*PI*1./(2*NEG_PW)*(dt))))
#define RC_SIN_FUNC_DOWN (PWM_RANGE * (sin(2*PI*0.8/(2*NEG_PW)*(dt))))

int getDutyCycle(int dt, int mode)
{
  if (mode == 0) // rectangular
    return (int)(PWM_RANGE*STD_DUTY_CYCLE);
  else if (mode == 1) // exponential
    return (int)RC_EXP_FUNC;
  else if (mode == 2) // sinusoidal
    if (dt < NEG_PW/2)
      return RC_SIN_FUNC_UP;
    else
      return RC_SIN_FUNC_DOWN;     
}
