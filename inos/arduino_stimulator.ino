/* *
 * Code based on Instructables tutorial: http://www.instructables.com/id/Modify-The-HC-05-Bluetooth-Module-Defaults-Using-A/?ALLSTEPS
 * For the HC05 board used, the pushbutton was be pressed while powering it on.
 * And on the reference sheet: http://www.instructables.com/files/orig/FOR/4FP2/HKZAVRT6/FOR4FP2HKZAVRT6.pdf 
 * 
 * This script enables communication with the HC05 board while in AT mode
 * using Arduino IDE's serial monitor.
 *  */

#include <SoftwareSerial.h>
#include <string.h>

/* Pin Initialization */
#define NEG_PIN 2
#define POS_PIN 4
#define PWM_PIN 3
#define PWM_PERIOD 33
#define LED_PIN 13
SoftwareSerial BTSerial(8,9);

/* Variable declaration */
float STD_DUTY_CYCLE;
long NEG_PW; // us
long POS_PW; // us
long INTERPHASE_DELAY; // us (offset de 30us)
long MAIN_PERIOD; // us
long INTERPULSE_DELAY; // us
long PWM_MODE; // 0: rectangular; 1: exponential; 2: sinusoidal
long PWM_RANGE;
long NEG_TIME_OFFSET; // us
long POS_TIME_OFFSET; // us
float EXP; // expoente


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
void update_stimulator(String msg);

void setup()
{
  // Changing HC05 to AT mode (if button is not present in board)
//  pinMode(7, OUTPUT);
//  digitalWrite(7, HIGH);

/* Variables initialization */
  STD_DUTY_CYCLE = 0.9;
  NEG_PW = 700; // us
  POS_PW = 500; // us
  INTERPHASE_DELAY = 0; // us (offset de 30us)
  MAIN_PERIOD = 4000;
  INTERPULSE_DELAY = MAIN_PERIOD - NEG_TIME_OFFSET - NEG_PW - INTERPHASE_DELAY - POS_PW; // us
  PWM_MODE = 0; // 0: rectangular; 1: exponential; 2: sinusoidal
  PWM_RANGE = 256;
  NEG_TIME_OFFSET = 50; // us
  POS_TIME_OFFSET = 50; // us
  EXP = -.0025;
  
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
  
  Serial.begin(9600);
  BTSerial.begin(9600);
  BTSerial.setTimeout(2);
  Serial.println("Enter AT commands:");
}

void loop()
{
  /* Pulses loop */
  if (old_status != curr_status)
  {
    t_this_status = micros();
    old_status = curr_status;
  }
    
  /* A little before first pulse */
  if (curr_status == before_first_pulse)
  {
    if (PWM_MODE == 0)
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
      
    curr_status = inter_phase;
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
      
      dc = getDutyCycle((t_now - t_this_status), PWM_MODE);
      
      if (dc < 0)
      dc = 255-dc;
      analogWrite(PWM_PIN, dc);

      delayMicroseconds(10);
      t_now = micros();
      
    }
    digitalWrite(POS_PIN, LOW); 
    
    curr_status = inter_pulse;
  }
    
  /* Interpulse */
  else if (curr_status == inter_pulse)
  {
    if (PWM_MODE == 0)
      analogWrite(PWM_PIN, (int)(STD_DUTY_CYCLE*PWM_RANGE));
    else
      analogWrite(PWM_PIN, 255);
    if (INTERPULSE_DELAY < 16000)
    {
      delayMicroseconds(INTERPULSE_DELAY);
    }
    else
    {
      delay(INTERPULSE_DELAY/1000);
    }
    
    curr_status = before_first_pulse;
  }


  /* Communication loop */
  if (BTSerial.available() > 0)
  {
    String received_msg = BTSerial.readString();
//    Serial.write(received_byte);
//    Serial.println(received_msg);
    update_stimulator(received_msg);
  }
  if (Serial.available() > 0 )
  {
    byte received_byte = Serial.read();
    BTSerial.write(received_byte);
  }
}


#define RC_CANCEL_FUNC (PWM_RANGE * (1-exp(-.0001*dt)))
//#define RC_EXP_FUNC 
#define RC_SIN_FUNC_UP (PWM_RANGE * (sin(2*PI*1./(2*NEG_PW)*(dt))))
#define RC_SIN_FUNC_DOWN (PWM_RANGE * (sin(2*PI*0.8/(2*NEG_PW)*(dt))))

int getDutyCycle(int dt, int mode)
{
  if (mode == 0) // rectangular
    return (long)(PWM_RANGE*STD_DUTY_CYCLE);
  else if (mode == 1) // exponential
    return (long)(PWM_RANGE * (exp(EXP*dt)));
  else if (mode == 2) // sinusoidal
    if (dt < NEG_PW/2)
      return RC_SIN_FUNC_UP;
    else
      return RC_SIN_FUNC_DOWN;     
}


void update_stimulator(String msg)
{
  Serial.println(msg);
  char* str;
  str = (char*)malloc(msg.length()*sizeof(char));
  msg.toCharArray(str,msg.length());
  const char s[2] = ";";
  char* channels;
  char* pw_pos;
  char* pw_neg;
  char* interphase;
  char* freq;
  char* polarity;
  char* wave;
  char* amplitude;
  char* expoente;

  channels = strtok (str, s);
  pw_pos = strtok (NULL, s);
  pw_neg = strtok (NULL, s);
  interphase = strtok (NULL, s);
  freq = strtok (NULL, s);
  polarity = strtok (NULL, s);
  wave = strtok (NULL, s);
  amplitude = strtok (NULL, s);
  expoente = strtok (NULL, s);


  /* Update values */
  STD_DUTY_CYCLE = 1-atol(amplitude)/((float)100);
  NEG_PW = atol(pw_neg); // us
  if (atol(polarity) == 0)
  {
    POS_PW = 0;
  }
  else
  {
    POS_PW = atol(pw_pos); // us
  }
  INTERPHASE_DELAY = atol(interphase); // us (offset de 30us)
  MAIN_PERIOD = 1000000/atol(freq);
  INTERPULSE_DELAY = MAIN_PERIOD - NEG_TIME_OFFSET - NEG_PW - INTERPHASE_DELAY - POS_PW; // us
  PWM_MODE = atol(wave); // 0: rectangular; 1: exponential; 2: sinusoidal
  PWM_RANGE = 256;
  NEG_TIME_OFFSET = 50; // us
  POS_TIME_OFFSET = 50; // us
  EXP = (float)atof(expoente);

//  Serial.println(POS_PW);
//  Serial.println(NEG_PW);
//  Serial.println(INTERPHASE_DELAY);
//  Serial.println(pw_pos);
//  Serial.println(pw_neg);
//  Serial.println(interphase);
  Serial.println(INTERPULSE_DELAY);
}

