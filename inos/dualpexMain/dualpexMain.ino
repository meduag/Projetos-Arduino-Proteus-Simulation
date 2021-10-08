#include <SoftwareSerial.h>
#include <SPI.h>

/* Standard stim configuration */
#define STD_DUTY_CYCLE 0.5
#define NEG_PW 700 // us
#define POS_PW 500 // us
#define INTERPHASE_DELAY 0 // us (offset de 30us)
#define NEG_TIME_OFFSET 50 // us
#define POS_TIME_OFFSET 50 // us

/* Definitions */
// Hardware setup
#define NEG_PIN 2
#define POS_PIN 4
#define PWM_PIN 3
#define PWM_PERIOD 33
#define LED_PIN 13

#define X_ACCEL_PIN A0
#define Y_ACCEL_PIN A1
#define X_ACCEL_PIN A2

#define DAC_OP_MODE 1
#define DAC_SS_PIN 10

// Communication protocol setup
#define COMM_COMMAND_START_BYTE   0x23
#define COMM_RESPONSE_START_BYTE  0x24
#define COMM_STREAM_START_BYTE    0x25

#define COMM_END_BYTE 0x3B

#define COMM_COMMAND_FINISH_OPERATION 0xF0

#define COMM_COMMAND_MESSAGE_DIAGNOSIS  0x20
#define COMM_COMMAND_MESSAGE_THERAPY    0x30

#define COMM_COMMAND_SUBTYPE_CHRONAXIE    0x01
#define COMM_COMMAND_SUBTYPE_RHEOBASE     0x02
#define COMM_COMMAND_SUBTYPE_ACCOMODATION 0x03
#define COMM_COMMAND_SUBTYPE_SCIENCE      0x04 // not yet supported

#define COMM_RESPONSE_SUBTYPE_START_DIAGNOSIS 0x20
#define COMM_RESPONSE_SUBTYPE_START_THERAPY   0x30

#define COMM_RESPONSE_RESULT_OK                     0x00
#define COMM_RESPONSE_RESULT_UNSUPPORTED_CHANNELS   0x02
#define COMM_RESPONSE_RESULT_BAD_CHANNELS           0x03 // not yet supported

#define COMM_RESPONSE_FINISH_OPERATION 0xF0

#define COMM_STREAM_SUBTYPE_DIAGOSIS  0x10
#define COMM_STREAM_SUBTYPE_THERAPY   0x20

#define COMM_STREAM_STATUS_STIMULATING  0x00
#define COMM_STREAM_STATUS_FINISHED     0x01
#define COMM_STREAM_STATUS_ERROR        0x02 // not yet supported

/* Variable declaration */
enum diagnosis_type_t
{
  idle,
  chronaxie,
  rheobase,
  accommodation,
  science
} diagnosis_type;
enum session_type_t
{
  none,
  diagnosis,
  therapy
} session_type;
enum waveform_type_t
{
  rectangular,
  exponential,
  sinusoidal
} waveform_type;
enum pulse_status_t
{
  first_pulse,
  inter_phase,
  second_pulse,
  inter_pulse,
  no_pulse
} old_status, curr_status;
int leave_loop;
int* channels;
int num_channels;
byte *in_message;
int t_now, t_this_status, t_this_PWM;
float* pulse_widths;
float* amplitudes;
boolean is_contracted;
float mean_values[3] = {0, 0, 0};
int mean_counter;
float* measured_data;
float* rheobase_amplitude;

/* Function declaration */
void initializeSession();
byte* readBTMessage();
int getBTSessionType();
int getBTDiagnosisType();
int getBTTherapyType();
void startSession();
void startDiagnosisResponse();
void startTherapyResponse();
void streamDiagnosisMessage(float* measured_data);
void streamTherapyMessage(float* measured_data);
void sendBTData(byte* out, int len);
void finishCommunication();
void streamDiagnosisFinishMessage();
boolean getFinishCommand();

/* Stimulation functions */
void stimulationPulse(float max_amplitude, float pulse_width);
uint16_t getCurrentAmplitudeValue(int dt, int polarity, float max_amplitude, float period);
void startChronaxie();
void startRheobase();
void startAccommodation();
void sendStimValue(int address, int operation_mode, uint16_t value);

/* Accelerometer functions */
float getFilteredSignal(int pin);
boolean detectMovement(float accel, float mean);

/* First run */
void setup()
{
  /* Variable initialization */
  diagnosis_type = idle;
  session_type = none;
  waveform_type = rectangular;
  old_status = no_pulse;
  curr_status = inter_pulse;
  is_contracted = false;
  leave_loop = 0;
  num_channels = 0;
  rheobase_amplitude = NULL;

  /* System initialization */
  pinMode(PWM_PIN, OUTPUT);
  pinMode(NEG_PIN, OUTPUT);
  pinMode(POS_PIN, OUTPUT);

  Serial1.begin(9600);
  Serial.begin(9600);

  pinMode(DAC_SS_PIN, OUTPUT);
  digitalWrite(DAC_SS_PIN, HIGH);
  SPI.begin();

  pinMode(X_ACCEL_PIN, INPUT);
  pinMode(Y_ACCEL_PIN, INPUT);
  pinMode(X_ACCEL_PIN, INPUT);

  sendStimValue(0, 1, (uint16_t)2048);
}

/* Eternal loop */
void loop()
{
  initializeSession();
  startSession();
}

void initializeSession()
{
  while (leave_loop == 0)
  {
    in_message = readBTMessage();
    
    if (in_message[0] != COMM_COMMAND_START_BYTE)
    {
      delay(100);
      continue;
    }

    Serial.println("START");

    Serial.print("Session type: ");
    if (getBTSessionType())
      continue;

    if (session_type == diagnosis)
    {
      if (!getBTDiagnosisType())
        startDiagnosisResponse();
    }
    else if (session_type == therapy)
    {
      if (!getBTTherapyType()) 
        startTherapyResponse();
    }
  }
  
  leave_loop = 0;
}

void startSession()
{
  // depending on diagnosis mode, act accordingly
  if (diagnosis_type == rheobase)
    startRheobase();
  else if (diagnosis_type == chronaxie)
    startChronaxie();
  else if (diagnosis_type == accommodation)
    startAccommodation();
  else
  {
    while (leave_loop == 0)
    {
      // stay here stimulating with parameters
      // get parameters from BlueTooth
    }

    leave_loop = 0;
  }
}

byte* readBTMessage()
{
  byte* in_bytes;
  in_bytes = (byte*) malloc(1*sizeof(byte));
  in_bytes[0] = 0;
  int iterator = 0;
  boolean reading_message;

  while (Serial1.available() > 0)
  {
    if (iterator != 0)
      in_bytes = (byte*) realloc(in_bytes, (iterator+1)*sizeof(byte));
      
    in_bytes[iterator] = Serial1.read();
    
    if (in_bytes[iterator] == COMM_END_BYTE)
    {
      reading_message = false;
      break;
    }
    else if (in_bytes[iterator] == COMM_COMMAND_START_BYTE || 
             in_bytes[iterator] == COMM_RESPONSE_START_BYTE ||
             in_bytes[iterator] == COMM_STREAM_START_BYTE)
    {
      reading_message = true;
      iterator++;
    }
    else if (reading_message)
      iterator++;
  }

  return in_bytes;
}

int getBTSessionType()
{
  if (in_message[1] == COMM_COMMAND_MESSAGE_DIAGNOSIS)
  {
    Serial.println("Diagnosis");
    session_type = diagnosis;
    return 0;
  }
  else if (in_message[1] == COMM_COMMAND_MESSAGE_THERAPY)
  {
    Serial.println("Therapy");
    session_type = therapy;
    return 0;
  }
  else
  {
    Serial.println("Else");
    return -1;
  }
}

int getBTDiagnosisType()
{
  if (in_message[2] < 2) // data length must be 1 (diagnosis type) + num_channels
    return -1;

  num_channels = in_message[2] - 1;
  channels = (int*) malloc(num_channels*sizeof(int));

  for (int i = 0; i < num_channels; i++)
    channels[i] = in_message[4+i];

  Serial.print("Number of channels: ");
  Serial.println(num_channels);
  Serial.print("Diagnosis: ");
  
  if (in_message[3] == COMM_COMMAND_SUBTYPE_RHEOBASE)
  {
    Serial.println("Rheobase");
    diagnosis_type = rheobase;
    leave_loop = 1;
    startDiagnosisResponse();
    return 0;
  }
  else if (in_message[3] == COMM_COMMAND_SUBTYPE_CHRONAXIE)
  {
    Serial.println("Chronaxie");
    diagnosis_type = chronaxie;
    leave_loop = 1;
    startDiagnosisResponse();
    return 0;
  }
  else if (in_message[3] == COMM_COMMAND_SUBTYPE_ACCOMODATION)
  {
    Serial.println("Accommodation");
    diagnosis_type = accommodation;
    leave_loop = 1;
    startDiagnosisResponse();
    return 0;
  }
  else if (in_message[3] == COMM_COMMAND_SUBTYPE_SCIENCE)
  {
    diagnosis_type = science;
    leave_loop = 1;
    startDiagnosisResponse();
    return 0;
  }
  else
    diagnosis_type = idle;

  return -1;
}

int getBTTherapyType()
{
  return -1;
}

void startDiagnosisResponse()
{
  byte* response;
  int response_length;
  
  int* unsupported_channels;
  int num_unsupported_channels = 0;
  int iterator;

  unsupported_channels = (int*) malloc(0*sizeof(int));

  // Finding which channels are unsupported
  for (iterator = 0; iterator < num_channels; iterator++)
  {
    if (channels[iterator] < 1 || channels[iterator] > 4)
    {
      unsupported_channels = (int*) realloc(unsupported_channels, sizeof(int));
      unsupported_channels[num_unsupported_channels] = iterator;
      num_unsupported_channels++;
    }
  }

  // Assemblying response
  response_length = 6+num_unsupported_channels;
  if (num_unsupported_channels)
  {
    response = (byte*) malloc(response_length*sizeof(byte));
    response[0] = COMM_RESPONSE_START_BYTE;
    response[1] = COMM_RESPONSE_SUBTYPE_START_DIAGNOSIS;
    response[2] = num_unsupported_channels + 1;
    response[3] = COMM_RESPONSE_RESULT_UNSUPPORTED_CHANNELS;

    // Unsupported channels listing
    for (iterator = 0; iterator < num_unsupported_channels; iterator++)
      response[4+iterator] = unsupported_channels[iterator];

    // Checksum
    response[4+num_unsupported_channels] = 0;
    for (iterator = 1; iterator < 3 + num_unsupported_channels; iterator++)
      response[4+num_unsupported_channels] += response[iterator];
      
    response[4+num_unsupported_channels+1] = COMM_END_BYTE;
  }
  else
  {
    response = (byte*) malloc(response_length*sizeof(byte));
    response[0] = COMM_RESPONSE_START_BYTE;
    response[1] = COMM_RESPONSE_SUBTYPE_START_DIAGNOSIS;
    response[2] = num_unsupported_channels + 1;
    response[3] = COMM_RESPONSE_RESULT_OK;

    // Checksum
    response[4] = 0;
    for (iterator = 1; iterator < 4; iterator++)
      response[4] += response[iterator];
      
    response[5] = COMM_END_BYTE;
  }

  // Sending response
  sendBTData(response, response_length);
}

void startTherapyResponse()
{
}

void streamDiagnosisMessage(float* measured_data)
{
  byte* message;
  int message_length;
  
  int iterator;
  union
  {
    float the_float;
    byte the_bytes[4];
  } float_and_bytes;

  message_length = (7+num_channels*6);
  message = (byte*) malloc(message_length*sizeof(byte));
  
  // Assemblying message
  message[0] = COMM_STREAM_START_BYTE;
  message[1] = COMM_STREAM_SUBTYPE_DIAGOSIS;
  message[2] = 2+num_channels*6;
  
  if (diagnosis_type == chronaxie)
    message[3] = COMM_COMMAND_SUBTYPE_CHRONAXIE;
  else if (diagnosis_type == rheobase)
    message[3] = COMM_COMMAND_SUBTYPE_RHEOBASE;
  else if (diagnosis_type == accommodation)
    message[3] = COMM_COMMAND_SUBTYPE_ACCOMODATION;
  else if (diagnosis_type == science)
    message[3] = COMM_COMMAND_SUBTYPE_SCIENCE;

  message[4] = COMM_STREAM_STATUS_STIMULATING;

  for (iterator = 0; iterator < num_channels; iterator++)
  {
    message[5+(iterator*6)] = channels[iterator];
    message[6+(iterator*6)] = COMM_RESPONSE_RESULT_OK; // check before if channel is ok (not yet supported)
    
    float_and_bytes.the_float = measured_data[iterator];
    
    message[7+(iterator*6)]   = float_and_bytes.the_bytes[3];
    message[8+(iterator*6)]   = float_and_bytes.the_bytes[2];
    message[9+(iterator*6)]   = float_and_bytes.the_bytes[1];
    message[10+(iterator*6)]  = float_and_bytes.the_bytes[0];
  }

  // checksum
  message[5+(num_channels*6)] = 0;
  for (iterator = 1; iterator < 5+(num_channels*6); iterator++)
    message[5+(num_channels*6)] += message[iterator];

  message[5+(num_channels*6)+1] = COMM_END_BYTE;

  // Sending message
  sendBTData(message, message_length);
}

void streamTherapyMessage(float* measured_data)
{
}

void sendBTData(byte* out, int len)
{
  Serial.print("Arduino: ");
  
  for (int i = 0; i < len; i++)
  {
    Serial1.write(out[i]);
  }
  for (int i = 0; i < len; i++)
  {
    if (i != 0)
      Serial.print(", ");
    Serial.print(out[i], HEX);
  }
  Serial.println();
}

void finishCommunication()
{
  streamDiagnosisFinishMessage();
  
  if (getFinishCommand())
  {
    Serial.println("Finish command received!");
    sendFinishResponse();
  }
}

void streamDiagnosisFinishMessage()
{
  byte* message;
  int message_length;
  
  int iterator;

  union
  {
    float the_float;
    byte the_bytes[4];
  } float_and_bytes;

  message_length = (7+num_channels*6);
  message = (byte*) malloc(message_length*sizeof(byte));
  
  // Assemblying message
  message[0] = COMM_STREAM_START_BYTE;
  message[1] = COMM_STREAM_SUBTYPE_DIAGOSIS;
  message[2] = 2+num_channels*6;
  
  if (diagnosis_type == chronaxie)
    message[3] = COMM_COMMAND_SUBTYPE_CHRONAXIE;
  else if (diagnosis_type == rheobase)
    message[3] = COMM_COMMAND_SUBTYPE_RHEOBASE;
  else if (diagnosis_type == accommodation)
    message[3] = COMM_COMMAND_SUBTYPE_ACCOMODATION;
  else if (diagnosis_type == science)
    message[3] = COMM_COMMAND_SUBTYPE_SCIENCE;

  message[4] = COMM_STREAM_STATUS_FINISHED;

  for (iterator = 0; iterator < num_channels; iterator++)
  {
    message[5+(iterator*6)] = channels[iterator];
    message[6+(iterator*6)] = COMM_STREAM_STATUS_FINISHED; // check before if channel is ok (not yet supported)
   
    float_and_bytes.the_float = measured_data[iterator];
    
    message[7+(iterator*6)]   = float_and_bytes.the_bytes[3];
    message[8+(iterator*6)]   = float_and_bytes.the_bytes[2];
    message[9+(iterator*6)]   = float_and_bytes.the_bytes[1];
    message[10+(iterator*6)]  = float_and_bytes.the_bytes[0];
  }

  // checksum
  message[5+(num_channels*6)] = 0;
  for (iterator = 1; iterator < 5+(num_channels*6); iterator++)
    message[5+(num_channels*6)] += message[iterator];

  message[5+(num_channels*6)+1] = COMM_END_BYTE;

  // Sending message
  sendBTData(message, message_length);
}

boolean getFinishCommand()
{
  byte* finish_message;
  finish_message = readBTMessage();
  
  int i = 0;
  while (i < 7)
  {
    if (i != 0)
      Serial.print(", ");
    Serial.print(finish_message[i], HEX);
    i++;
  }
  Serial.println();

  if (finish_message[1] == COMM_COMMAND_FINISH_OPERATION)
    return true;
  
  return false;
}

void sendFinishResponse()
{
  byte* response;
  int response_length;
  int iterator;

  response_length = 6;
  
  response = (byte*) malloc(response_length*sizeof(byte));
  response[0] = COMM_RESPONSE_START_BYTE;
  response[1] = COMM_RESPONSE_FINISH_OPERATION;
  response[2] = 1;
  response[3] = COMM_RESPONSE_RESULT_OK;

  // Checksum
  response[4] = 0;
  for (iterator = 1; iterator < 4; iterator++)
    response[4] += response[iterator];
    
  response[5] = COMM_END_BYTE;

  // Sending response
  sendBTData(response, response_length);
}

/* Stimulation functions */
/* User configuration */
#define STD_DUTY_CYCLE 0.5
#define NEG_PW 700 // us
#define INTERPHASE_DELAY 0 // us (offset de 30us)
#define INTERPULSE_DELAY 2000 // ms
#define STIM_MAX 4096
#define STIM_MIN 0
#define STIM_ZERO 2048
#define NEG_TIME_OFFSET 50 // us
#define POS_TIME_OFFSET 50 // us
#define POSITIVE_POLARITY 1
#define NEGATIVE_POLARITY -1

#define RHEOBASE_PULSE_WIDTH 1000

void stimulationPulse(float max_amplitude, float pulse_width)
{
  uint16_t stim_value;

  old_status = no_pulse;
  curr_status = first_pulse;
  
  while (old_status != inter_pulse)
  {
    if (old_status != curr_status)
    {
      t_this_status = millis();
      old_status = curr_status;
    }
      
    /* First pulse */
    if (curr_status == first_pulse)
    {
      t_now = millis();
      while ((t_now - t_this_status) < pulse_width)
      {
        stim_value = getCurrentAmplitudeValue((t_now-t_this_status), POSITIVE_POLARITY, max_amplitude, pulse_width);
        sendStimValue(0, DAC_OP_MODE, stim_value);

        checkContraction();
        t_now = millis();
      }
      
      sendStimValue(0, DAC_OP_MODE, (uint16_t)(STIM_ZERO));
        
      curr_status = inter_pulse;
    }
      
    /* Interphase */
    else if (curr_status == inter_phase)
    {
      t_now = millis();
      while ((t_now - t_this_status) < INTERPHASE_DELAY)
      {
        checkContraction();
        t_now = millis();
      }
      
      curr_status = second_pulse;
    }
      
    /* Second pulse */
    else if (curr_status == second_pulse)
    {
      t_now = millis();
      while ((t_now - t_this_status) < pulse_width)
      {
        stim_value = getCurrentAmplitudeValue((t_now-t_this_status), NEGATIVE_POLARITY, max_amplitude, pulse_width);
        sendStimValue(0, DAC_OP_MODE, stim_value);

        checkContraction();
        t_now = millis();
      }

      sendStimValue(0, DAC_OP_MODE, (uint16_t)STIM_ZERO);
      
      curr_status = inter_pulse;
    }
      
    /* Interpulse */
    else if (curr_status == inter_pulse)
    {
      t_now = millis();
      while ((t_now - t_this_status) < INTERPULSE_DELAY)
      {
        checkContraction();
        t_now = millis();
      }
      
      curr_status = first_pulse;
    }
  }
}

#define NORM_EXP(TIME_SCALE, AMP_SCALE) (exp(0.6931*dt/TIME_SCALE) - 1)*AMP_SCALE
#define RC_SIN_FUNC_UP (max_amplitude * (sin(2*PI*1./(2*NEG_PW)*(dt))))
#define RC_SIN_FUNC_DOWN (max_amplitude * (sin(2*PI*0.8/(2*NEG_PW)*(dt))))

uint16_t getCurrentAmplitudeValue(int dt, int polarity, float max_amplitude, float period)
{
  uint16_t stim = 0;

  if (waveform_type == rectangular)
    stim = (uint16_t)((STIM_MAX-STIM_ZERO) * max_amplitude);

  else if (waveform_type == exponential)
  {
    stim = (uint16_t)(NORM_EXP(period,(STIM_MAX-STIM_ZERO)*max_amplitude));
  }

  else if (waveform_type == sinusoidal)
  {
    if (dt < NEG_PW/2)
      stim = (uint16_t)RC_SIN_FUNC_UP;
    else
      stim = (uint16_t)RC_SIN_FUNC_DOWN;     
  }
  
  return (polarity*(stim - STIM_ZERO));
}

void startRheobase()
{
  measured_data = (float*) malloc(num_channels*sizeof(float));
  int iterator;

  amplitudes = (float*) malloc(num_channels*sizeof(float));
  for (iterator = 0; iterator < num_channels; iterator++)
    amplitudes[iterator] = 0;
  waveform_type = rectangular;

  // Get mean value for threshold
  Serial.println("Starting rheobase callibration. Please stand still for 5 seconds.");
  for (iterator = 0; iterator < 3; iterator++)
    mean_values[iterator] = 0;
  mean_counter = 0;
  int t0 = millis();
  while (millis() - t0 < 4000)
  {
    mean_values[0] += getFilteredSignal(X_ACCEL_PIN);
    mean_values[1] += getFilteredSignal(Y_ACCEL_PIN);
    mean_values[2] += getFilteredSignal(X_ACCEL_PIN);
    mean_counter++;
  }

  mean_values[0] /= mean_counter;
  mean_values[1] /= mean_counter;
  mean_values[2] /= mean_counter;

  Serial.print("Callibration done! With samples: ");
  Serial.println(mean_counter);
 
  while (!is_contracted)
  {
    // increase amplitude
    for (iterator = 0; iterator < num_channels; iterator++)
      amplitudes[iterator] += 0.05; // us
      
    // stimulate
    Serial.println("Amplitude: ");
    Serial.println(amplitudes[0]);
    stimulationPulse(amplitudes[0], RHEOBASE_PULSE_WIDTH); // TODO: change stimulationPulse to multiple channels
      
    // check if operation was finished by other side
    
    // stream diagnosis response
    Serial.println("Streaming...");
    for (iterator = 0; iterator < num_channels; iterator++)
      measured_data[iterator] = amplitudes[iterator]*4096;
    streamDiagnosisMessage(measured_data);
  }

  Serial.println("Contraction detected, finishing...");
  rheobase_amplitude = (float*)malloc(sizeof(float));
  is_contracted = false;

  finishCommunication();
}

void startChronaxie()
{
  if (rheobase_amplitude == NULL)
  {
    // creating standard rheobase amplitude for debugging reasons
    rheobase_amplitude = (float*)malloc(sizeof(float));
    *rheobase_amplitude = 0.5;

    // TODO: finish operation (send error) if rheobase_amplitude == NULL
  }
    
  measured_data = (float*) malloc(num_channels*sizeof(float));
  int iterator;

  pulse_widths = (float*) malloc(num_channels*sizeof(float));
  for (iterator = 0; iterator < num_channels; iterator++)
    pulse_widths[iterator] = 0;
  waveform_type = rectangular;

  // Get mean value for threshold
  Serial.println("Starting chronaxie callibration. Please stand still for 5 seconds.");
  for (iterator = 0; iterator < 3; iterator++)
    mean_values[iterator] = 0;
  mean_counter = 0;
  int t0 = millis();
  while (millis() - t0 < 4000)
  {
    mean_values[0] += getFilteredSignal(X_ACCEL_PIN);
    mean_values[1] += getFilteredSignal(Y_ACCEL_PIN);
    mean_values[2] += getFilteredSignal(X_ACCEL_PIN);
    mean_counter++;
  }

  mean_values[0] /= mean_counter;
  mean_values[1] /= mean_counter;
  mean_values[2] /= mean_counter;

  Serial.print("Callibration done! With samples: ");
  Serial.println(mean_counter);
 
  while (!is_contracted)
  {
    Serial.println("Inside loop!");
    // increase pulse width
    for (iterator = 0; iterator < num_channels; iterator++)
      pulse_widths[iterator] += 10; // ms
      
    // stimulate
    Serial.println("Pulse width: ");
    Serial.println(pulse_widths[0]);
    stimulationPulse(amplitudes[0], pulse_widths[0]); // TODO: change stimulationPulse to multiple channels
      
    // check if operation was finished by other side
    
    // stream diagnosis response
    Serial.println("Streaming...");
    for (iterator = 0; iterator < num_channels; iterator++)
      measured_data[iterator] = pulse_widths[iterator];
    streamDiagnosisMessage(measured_data);
  }

  Serial.println("Contraction detected, finishing...");
  is_contracted = false;

  finishCommunication();
}

void startAccommodation()
{
  measured_data = (float*) malloc(num_channels*sizeof(float));
  int iterator;

  amplitudes = (float*) malloc(num_channels*sizeof(float));
  for (iterator = 0; iterator < num_channels; iterator++)
    amplitudes[iterator] = 0;
  waveform_type = exponential;

  // Get mean value for threshold
  Serial.println("Starting accommodation callibration. Please stand still for 5 seconds.");
  for (iterator = 0; iterator < 3; iterator++)
    mean_values[iterator] = 0;
  mean_counter = 0;
  int t0 = millis();
  while (millis() - t0 < 4000)
  {
    mean_values[0] += getFilteredSignal(X_ACCEL_PIN);
    mean_values[1] += getFilteredSignal(Y_ACCEL_PIN);
    mean_values[2] += getFilteredSignal(X_ACCEL_PIN);
    mean_counter++;
  }

  mean_values[0] /= mean_counter;
  mean_values[1] /= mean_counter;
  mean_values[2] /= mean_counter;

  Serial.print("Callibration done! With samples: ");
  Serial.println(mean_counter);
 
  while (!is_contracted)
  {
    // increase maximum amplitude
    for (iterator = 0; iterator < num_channels; iterator++)
      amplitudes[iterator] += 0.05; 
      
    // stimulate
    Serial.println("Amplitude: ");
    Serial.println(amplitudes[0]);
    stimulationPulse(amplitudes[0], RHEOBASE_PULSE_WIDTH); // TODO: change stimulationPulse to multiple channels
      
    // check if operation was finished by other side
    
    // stream diagnosis response
    Serial.println("Streaming...");
    for (iterator = 0; iterator < num_channels; iterator++)
      measured_data[iterator] = amplitudes[iterator]*4.096;
    streamDiagnosisMessage(measured_data);
  }

  Serial.println("Contraction detected, finishing...");
  is_contracted = false;

  finishCommunication();
}

void sendStimValue(int address, int operation_mode, uint16_t value)
{
  byte valueToWriteH = 0;
  byte valueToWriteL = 0;
  
  valueToWriteH = highByte(value);
  valueToWriteH = 0b00001111 & valueToWriteH;
  valueToWriteH = (address << 6 ) | (operation_mode << 4) | valueToWriteH;
  valueToWriteL = lowByte(value);
  
  // take the SS pin low to select the chip:
  digitalWrite(DAC_SS_PIN, LOW);
  
  //  send in the address and value via SPI:
  SPI.transfer(valueToWriteH);
  SPI.transfer(valueToWriteL);
  
  // take the SS pin high to de-select the chip:
  digitalWrite(DAC_SS_PIN, HIGH);
}

/* Accelerometer functions */
float getFilteredSignal(int pin)
{
  float sensor_value = analogRead(pin);

  //  filter it

  return sensor_value;
}

boolean detectMovement(float accel, float mean)
{
  int threshold = 100;

  if (abs(accel - mean) > threshold)
  {
    Serial.println(String(accel-mean) + ">" + String(threshold));
    return true;
  }
  else 
    return false;
}

void checkContraction()
{
  float accel_x, accel_y, accel_z;

  // get accel response
  accel_x = getFilteredSignal(X_ACCEL_PIN);
  accel_y = getFilteredSignal(Y_ACCEL_PIN);
  accel_z = getFilteredSignal(X_ACCEL_PIN);

  // check if contraction
  if (detectMovement(accel_x, mean_values[0]) ||
      detectMovement(accel_y, mean_values[1]) ||
      detectMovement(accel_z, mean_values[2]))
    is_contracted = true;
}