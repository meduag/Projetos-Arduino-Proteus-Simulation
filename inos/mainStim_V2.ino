#include <SPI.h>

/* Definitions */
// Hardware setup

//definicion de los pinos para el acelerometro
#define ACCEL_PIN(x) ((x == 0) ? A0 : ((x == 1) ? A1 : ((x == 2) ? A2 : ((x == 3) ? A3 : -1))))

//define el inicio da grabaçao e sincronizacion de datos
#define REC_SYNC_PIN 3 // Pino disponivel para habilitar comunicacion/inicio grabacion externa 
#define STIM_PIN 4 // señal dato 

//configuracion datos del dac
#define DAC_OP_MODE 1
#define DAC_SS_PIN 10


//definicion pinos del hardware
#define RELAY_CH_1 9 //rele del canal 1
#define RELAY_CH_2 8 //rele del canal 2
#define RELAY_CH_3 7 //rele del canal 3
#define RELAY_CH_4 6 //rele del canal 4
#define STOP_BUTTON 5 //Boton de emergencia

// Types
#define COMM_REQUISITION_START_BYTE   0x23 // Comando de inicio (Start ede RCA) - Cliente --> Estimulador
#define COMM_RESPONSE_START_BYTE      0x24 // Respuesta (Start ede RCA) - Estimulador --> Cliente
#define COMM_NOTIFICATION_START_BYTE  0x25 // Notificaciones en R/ a una requisicion devuelve finalizacion ede RCA- Estimulador --> Cliente ()
#define COMM_CONFIRMATION_START_BYTE  0x26 // Confirmaciones Cliente - Estimulador R/ a una notificacion 

// Sub types
#define COMM_RT_STOP_ALL                      0xF0 //Para todos los canales
#define COMM_RT_STOP_CHANNEL                  0xF1 //Para un canal especifico

/////Que son?
#define COMM_COMMAND_MESSAGE_STREAM           0x20 //
#define COMM_COMMAND_MESSAGE_STREAM_FINISH    0X21 //
#define COMM_COMMAND_MESSAGE_THERAPY          0x30 //
#define COMM_COMMAND_MESSAGE_DIAGNOSIS        0X40 //
#define COMM_COMMAND_MESSAGE_THERAPY_UPDATE   0x38 //

#define COMM_END_BYTE 0x3B //Byte finalizacion

//Subtipos
#define COMM_COMMAND_SUBTYPE_RHEOBASE     0x01
#define COMM_COMMAND_SUBTYPE_CHRONAXIE    0x02
#define COMM_COMMAND_SUBTYPE_ACCOMODATION 0x03


#define COMM_COMMAND_SUBTYPE_SCIENCE      0x04 // not yet supported

//
#define COMM_COMMAND_SUBTYPE_OPEN_LOOP    0x00
#define COMM_COMMAND_SUBTYPE_CLOSED_LOOP  0x01

// Responses
#define COMM_RESPONSE_RESULT_OK                     0x00
#define COMM_RESPONSE_RESULT_UNSUPPORTED_OPERATION  0x01
#define COMM_RESPONSE_RESULT_UNSUPPORTED_CHANNELS   0x02
#define COMM_RESPONSE_RESULT_BAD_CHANNELS           0x03 // not yet supported

#define COMM_RESPONSE_FINISH_OPERATION 0xF0

#define COMM_STREAM_SUBTYPE_DIAGOSIS  0x10
#define COMM_STREAM_SUBTYPE_THERAPY   0x20

#define COMM_STREAM_STATUS_STIMULATING  0x00
#define COMM_STREAM_STATUS_FINISHED     0x01
#define COMM_STREAM_STATUS_ERROR        0x02 // not yet supported


#define MAX_PACKET_SIZE 256
#define DATA_PACKET_SIZE_THERAPY 49
#define DATA_PACKET_SIZE_THERAPY_UPDATE 6
#define SERIAL_TIMEOUT  50
#define SERIAL1_TIMEOUT 50


/* Stimulation user configuration */
#define STIM_MAX          4095
#define STIM_MIN          0
#define STIM_ZERO         2047
#define POSITIVE_POLARITY 1
#define NEGATIVE_POLARITY -1
#define INTERPHASE_DELAY  0 // us (offset de 30us)
#define INTERPULSE_DELAY  2000000 // us
#define INTERCHANNEL_DELAY  500000 //us 

//#define RHEOBASE_PULSE_WIDTH 1000 // para electrodo monopolar
#define RHEOBASE_PULSE_WIDTH 500000 // para electrodo bipolar micro segundos us

#define BUFFER_SIZE 10

/* Variable declaration */
union
{
  float the_float;
  byte the_bytes[4];
} float_and_bytes;
enum diagnosis_type_t
{
  no_diagnosis,
  chronaxie,
  rheobase,
  accommodation,
  science
} diagnosis_type;
enum therapy_type_t
{
  no_therapy,
  open_loop,
  closed_loop
} therapy_type;
enum session_type_t
{
  no_session,
  diagnosis,
  therapy
} session_type;
enum waveform_type_t
{
  rectangular,
  exponential,
  sinusoidal
} waveform_type;
enum diagnosis_pulse_status_t
{
  diagnosis_no_pulse,
  first_pulse,
  inter_phase,
  second_pulse,
  inter_pulse
} diagnosis_old_status, diagnosis_curr_status;
enum therapy_macro_states_t
{
  therapy_no_pulse,
  min_amp,
  ramp_up,
  max_amp,
  ramp_down
};
enum control_type_t
{
  amplitude_modulated,
  pulse_width_modulated,
  frequency_modulated
};
// TODO: change array within struct to array of structs
struct diagnosis_t
{
  byte num_channels;
  int* channels;
  float* pulse_widths;
  float* amplitudes;
  boolean is_contracted;
  float accel_value = 0;
  int accel_counter;
  float* measured_data;
  float* rheobase_amplitude;
} d;
byte num_therapy_channels;
struct therapy_t
{
  int channel_id;
  float frequency; // Hz
  float amplitude;
  float pulse_width; // us
  float therapy_duration; // ms
  float error[2];
  float t_rest; // ms
  float t_stim; // ms
  float t_up; // ms
  float t_down; // ms
  float measured_data;
  float reference_data;
  long T_total; // ms
  long t_total; // ms
  long t_now; // ms
  enum therapy_macro_states_t macro_state;
  float K_P, K_D, K_I;
  float control_factor;
};
struct therapy_t* t;
boolean leave_loop;
byte *in_message;
int t_now, t_begin, t_this_status, t_this_PWM;
boolean start_session;
boolean stop_therapy;
boolean null_id;
float* circular_buffer;
int* buffer_iterator;
char message_id[4];

/* Function declaration */
/* System functions */
void initializeSession();
void startSession();
byte* readBTMessage(boolean verbose, int size);
int getBTSessionType();
int getBTDiagnosisType();
int getBTTherapyType();
boolean startDiagnosisResponse(boolean verbose);
boolean startTherapyResponse(boolean verbose);
boolean changeParamsResponse(boolean verbose);
boolean finishOperationResponse(boolean verbose);
void streamDiagnosisMessage(boolean verbose);
void streamTherapyMessage(boolean verbose);
void sendBTData(byte* out, int len, boolean verbose);
void finishCommunication();
void streamDiagnosisFinishMessage(boolean verbose);
void streamTherapyFinishMessage(boolean verbose);
boolean getFinishCommand();
void sendFinishResponse(boolean verbose);

/* Stimulation functions */
void diagnosisStimPulse(float max_amplitude, float pulse_width);
void openLoopStimPulse();
int changeParametersRT();
void closedLoopStimPulse(control_type_t control_type);
uint16_t controlAlgorithm(control_type_t control_type, int channel, float control_p, float control_i, float control_d, float DT);
void initControlParameters();
therapy_macro_states_t getMacroState(long sometime, int which_channel);
uint16_t getCurrentAmplitudeValue(int dt, int polarity, float max_amplitude, float period);
void startRheobase();
void startChronaxie();
void startAccommodation();
void startOpenLoop();
void startClosedLoop();
void sendStimValue(int address, int operation_mode, uint16_t value);
void zeroChannels();
boolean buttonStop();

/* Accelerometer functions */
float getFilteredSignal(int pin);
float getRMSAcceleration(int which_channel);
boolean detectMovement(float accel, float mean);
void checkContraction();

/* First run */
void setup()
{
  /* Variable initialization */
  diagnosis_type = no_diagnosis;
  therapy_type = no_therapy;
  session_type = no_session;
  waveform_type = rectangular;

  diagnosis_old_status = diagnosis_no_pulse;
  diagnosis_curr_status = inter_pulse;

  start_session = true;
  leave_loop = false;
  null_id = false;

  d.is_contracted = false;
  d.num_channels = 0;
  d.rheobase_amplitude = NULL;
  num_therapy_channels = 0;
  float_and_bytes.the_float = 0;

  circular_buffer = NULL;

  /* System initialization */
  // Communication
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial.setTimeout(SERIAL_TIMEOUT);
  Serial1.setTimeout(SERIAL1_TIMEOUT);

  // DAC
  pinMode(DAC_SS_PIN, OUTPUT);
  digitalWrite(DAC_SS_PIN, HIGH);
  SPI.begin();

  //Habilitaçao gravacion de dados
  pinMode(REC_SYNC_PIN, OUTPUT);
  digitalWrite(REC_SYNC_PIN, LOW);// inicia en bajo, no habilita gravacion de datos

  //Pino para mostrar inicio y final del pulso de estimulacion para diagnostico - sincroniza tiempo y punto de estimulacion
  pinMode(STIM_PIN, OUTPUT);
  digitalWrite(STIM_PIN, LOW);// comienza en bajo sin pulso

  // Stop button
  pinMode(STOP_BUTTON, INPUT);

  // Accelerometer
  pinMode(ACCEL_PIN(0), INPUT);
  pinMode(ACCEL_PIN(1), INPUT);
  pinMode(ACCEL_PIN(2), INPUT);
  pinMode(ACCEL_PIN(3), INPUT);

  // Relays
  pinMode(RELAY_CH_1, OUTPUT);
  pinMode(RELAY_CH_2, OUTPUT);
  pinMode(RELAY_CH_3, OUTPUT);
  pinMode(RELAY_CH_4, OUTPUT);
  digitalWrite(RELAY_CH_1, LOW);
  digitalWrite(RELAY_CH_2, LOW);
  digitalWrite(RELAY_CH_3, LOW);
  digitalWrite(RELAY_CH_4, LOW);
  // sendStimValue(0, DAC_OP_MODE, (uint16_t)(STIM_ZERO + 20.47));
  // sendStimValue(1, DAC_OP_MODE, (uint16_t)(STIM_ZERO + 20.47));
  // sendStimValue(2, DAC_OP_MODE, (uint16_t)(STIM_ZERO + 20.47));
  // sendStimValue(3, DAC_OP_MODE, (uint16_t)(STIM_ZERO + 20.47));

  Serial.println("hello world");

  zeroChannels();
}

/* Eternal loop */
void loop()
{
  // Stay here until the app sends a start command
  initializeSession(); // sets up channels and session types

  if (start_session)
    startSession(); // implements the session commanded

  session_type = no_session;
  therapy_type = no_therapy;
  diagnosis_type = no_diagnosis;
}

/* System functions */
void initializeSession()
{
  in_message = NULL;
  delay(100);
  Serial.println("Waiting for app...");
  zeroChannels();
  Serial.flush();
  Serial.println(Serial.availableForWrite());

  while (!leave_loop)
  {
    if (!Serial1.available())
      continue;
    in_message = NULL;
    Serial.println("Before reading...");
    in_message = readBTMessage(true, MAX_PACKET_SIZE);

    if (in_message[0] != COMM_REQUISITION_START_BYTE)
    {
      delay(100);
      continue;
    }
    Serial.println("After reading...");

    // TODO: check if this is necessary
    for (int iterator = 0; iterator < 80; iterator++)
    {
      Serial.print(in_message[iterator], HEX);
      Serial.print(" ");
    }
    Serial.println();

    Serial.println("START");

    Serial.print("Session type: ");
    if (getBTSessionType())
      continue;

    if (session_type == diagnosis)
    {
      if (!getBTDiagnosisType())
        start_session &= startDiagnosisResponse(true);
    }
    else if (session_type == therapy)
    {
      if (!getBTTherapyType())
        start_session &= startTherapyResponse(true);
    }
  }

  leave_loop = false;
}

void startSession()
{
  // Depending on diagnosis mode, act accordingly
  if (diagnosis_type != no_diagnosis)
  {
    digitalWrite(RELAY_CH_1, HIGH); // TODO: make this out of the microstate
    // digitalWrite(RELAY_CH_2, HIGH);
    // digitalWrite(RELAY_CH_3, HIGH);
    // digitalWrite(RELAY_CH_4, HIGH);

    // colocar aqui o sinal do pino de inicio de diagnostico
    digitalWrite(REC_SYNC_PIN, HIGH); //Habilita la gravacion de datos

    if (diagnosis_type == rheobase)
      startRheobase();
    else if (diagnosis_type == chronaxie)
      startChronaxie();
    else if (diagnosis_type == accommodation)
      startAccommodation();
    else if (diagnosis_type == science)
    {
      while (!leave_loop)
      {
        // stay here stimulating with parameters
        // get parameters from BlueTooth
      }
    }
    leave_loop = false;
  }
  else if (therapy_type != no_therapy)
  {
    if (therapy_type == open_loop)
      startOpenLoop();
    else if (therapy_type == closed_loop)
      startClosedLoop();
  }
}

byte* readBTMessage(boolean verbose, int size)
{
  byte* in_bytes = NULL;
  in_bytes = (byte*) malloc(size * sizeof(byte));
  in_bytes[0] = 0;

  if (Serial1.available() > 0)
    Serial1.readBytes(in_bytes, size);
  //Serial1.readBytes(in_bytes,Serial1.available());


  Serial.println(in_bytes[0], HEX);

  if (in_bytes[0] != COMM_REQUISITION_START_BYTE &&
      in_bytes[0] != COMM_CONFIRMATION_START_BYTE)
    in_bytes = NULL;

  if (verbose)
  {
    Serial.print("From BT(" + String(size) + "): ");
    for (int iterator = 0; iterator < size; iterator++)
    {
      Serial.print(in_bytes[iterator], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }

  return in_bytes;
}

int getBTSessionType()
{
  // if (in_message[5] == COMM_COMMAND_MESSAGE_DIAGNOSIS)
  // {
  //   Serial.println("Diagnosis");
  //   session_type = diagnosis;
  //   return 0;
  // }
  if (in_message[5] == COMM_COMMAND_SUBTYPE_RHEOBASE)
  {
    Serial.println("Diagnosis Rheobase");
    session_type = diagnosis;
    return 0;
  }
  if (in_message[5] == COMM_COMMAND_SUBTYPE_CHRONAXIE)
  {
    Serial.println("Diagnosis Chronaxie");
    session_type = diagnosis;
    return 0;
  }
  if (in_message[5] == COMM_COMMAND_SUBTYPE_ACCOMODATION)
  {
    Serial.println("Diagnosis Accommodation");
    session_type = diagnosis;
    return 0;
  }
  else if (in_message[5] == COMM_COMMAND_MESSAGE_THERAPY)
  {
    Serial.println("Therapy");
    session_type = therapy;
    return 0;
  }
  else if (in_message[5] == COMM_RT_STOP_ALL)
  {
    Serial.println("Finish operation");
    session_type = no_session;
    return 0;
  }
  else if (in_message[5] == COMM_RT_STOP_CHANNEL)
  {
    Serial.println("Finish channel operation");
    session_type = no_session;
    return 0;
  }
  else if (in_message[5] == COMM_COMMAND_MESSAGE_THERAPY_UPDATE)
  {
    Serial.println("Therapy update");
    session_type = therapy;
    return 0;
  }
  else
  {
    Serial.println("Session type unsupported");
    return -1;
  }
}

int getBTDiagnosisType()
{
  int iterator;

  // if (in_message[2] < 2) // data length must be 1 (diagnosis type) + num_channels
  //   return -1;

  d.num_channels = 1;//in_message[2] - 1;
  d.channels = (int*) malloc(d.num_channels * sizeof(int));

  for (iterator = 0; iterator < d.num_channels; iterator++)
    d.channels[iterator] = 0;//in_message[4+iterator] - 1;

  Serial.print("Number of channels: ");
  Serial.println(d.num_channels);
  Serial.print("Diagnosis: ");

  if (in_message[5] == COMM_COMMAND_SUBTYPE_RHEOBASE)
  {
    Serial.println("Rheobase");
    diagnosis_type = rheobase;
    leave_loop = true;
    return 0;
  }
  else if (in_message[5] == COMM_COMMAND_SUBTYPE_CHRONAXIE)
  {
    Serial.println("Chronaxie");
    diagnosis_type = chronaxie;
    leave_loop = true;
    return 0;
  }
  else if (in_message[5] == COMM_COMMAND_SUBTYPE_ACCOMODATION)
  {
    Serial.println("Accommodation");
    diagnosis_type = accommodation;
    leave_loop = true;
    return 0;
  }
  // else if (in_message[3] == COMM_COMMAND_SUBTYPE_SCIENCE)
  // {
  //   diagnosis_type = science;
  //   leave_loop = true;
  //   return 0;
  // }

  else {
    diagnosis_type = no_diagnosis;
    Serial.println("Diagnosis type not supported");
  }

  return -1;
}

int getBTTherapyType()
{
  int iterator, iterator_bytes, data_size;
  int data_start = 8;
  int freq_start = 8;
  int amp_start = 4;
  int pw_start = 12;
  int dur_start = 24;
  int rest_start = 20;
  int on_start = 16;
  int rise_start = 28;
  int decay_start = 32;
  int auto_start = 0;


  for (iterator_bytes = 0; iterator_bytes < 4; iterator_bytes++)
    message_id[iterator_bytes] = in_message[1 + iterator_bytes];

  data_size = in_message[7] + in_message[6] * 256;

  Serial.println(data_size);

  if (data_size < DATA_PACKET_SIZE_THERAPY) // data length must be 1 (therapy type) + (32+1)*num_channels
    return -1;

  num_therapy_channels = data_size / DATA_PACKET_SIZE_THERAPY;
  Serial.println("Number of channels: " + String(num_therapy_channels));

  t = (struct therapy_t*) malloc(num_therapy_channels * sizeof(struct therapy_t));

  for (iterator = 0; iterator < 80; iterator++)
  {
    Serial.print(in_message[iterator], HEX);
    Serial.print(" ");
  }
  Serial.println();

  for (iterator = 0; iterator < num_therapy_channels; iterator++)
  {
    /* Frequency */
    t[iterator].frequency = in_message[data_start + freq_start + DATA_PACKET_SIZE_THERAPY * iterator] * 16777216 +
                            in_message[data_start + freq_start + 1 + DATA_PACKET_SIZE_THERAPY * iterator] * 65536 +
                            in_message[data_start + freq_start + 2 + DATA_PACKET_SIZE_THERAPY * iterator] * 256 +
                            in_message[data_start + freq_start + 3 + DATA_PACKET_SIZE_THERAPY * iterator];
    // for (iterator_bytes = 8; iterator_bytes < 12; iterator_bytes++) {
    //   float_and_bytes.the_bytes[iterator_bytes] = in_message[8+iterator_bytes + DATA_PACKET_SIZE_THERAPY*iterator];
    //   Serial.println(in_message[8+iterator_bytes + DATA_PACKET_SIZE_THERAPY*iterator],HEX);
    // }
    // t[iterator].frequency = float_and_bytes.the_float;

    /* Amplitude */
    t[iterator].amplitude = in_message[data_start + amp_start + DATA_PACKET_SIZE_THERAPY * iterator] * 16777216 +
                            in_message[data_start + amp_start + 1 + DATA_PACKET_SIZE_THERAPY * iterator] * 65536 +
                            in_message[data_start + amp_start + 2 + DATA_PACKET_SIZE_THERAPY * iterator] * 256 +
                            in_message[data_start + amp_start + 3 + DATA_PACKET_SIZE_THERAPY * iterator];
    // Receive amplitude in uA, but work in mA.
    t[iterator].amplitude = t[iterator].amplitude * 20.47 / 1000;

    // Saturate amplitude
    if (t[iterator].amplitude > 2047)
      t[iterator].amplitude = 2047;
    // for (iterator_bytes = 4; iterator_bytes < 8; iterator_bytes++)
    //   float_and_bytes.the_bytes[iterator_bytes%4] = in_message[8+iterator_bytes + DATA_PACKET_SIZE_THERAPY*iterator];
    // t[iterator].amplitude = float_and_bytes.the_float;

    /* Pulse width */
    t[iterator].pulse_width = in_message[data_start + pw_start + DATA_PACKET_SIZE_THERAPY * iterator] * 16777216 +
                              in_message[data_start + pw_start + 1 + DATA_PACKET_SIZE_THERAPY * iterator] * 65536 +
                              in_message[data_start + pw_start + 2 + DATA_PACKET_SIZE_THERAPY * iterator] * 256 +
                              in_message[data_start + pw_start + 3 + DATA_PACKET_SIZE_THERAPY * iterator];
    // for (iterator_bytes = 12; iterator_bytes < 16; iterator_bytes++)
    //   float_and_bytes.the_bytes[iterator_bytes%4] = in_message[8+iterator_bytes + DATA_PACKET_SIZE_THERAPY*iterator];
    // t[iterator].pulse_width = float_and_bytes.the_float;

    /* Total time */
    t[iterator].therapy_duration = in_message[data_start + dur_start + DATA_PACKET_SIZE_THERAPY * iterator] * 16777216 +
                                   in_message[data_start + dur_start + 1 + DATA_PACKET_SIZE_THERAPY * iterator] * 65536 +
                                   in_message[data_start + dur_start + 2 + DATA_PACKET_SIZE_THERAPY * iterator] * 256 +
                                   in_message[data_start + dur_start + 3 + DATA_PACKET_SIZE_THERAPY * iterator];
    // for (iterator_bytes = 24; iterator_bytes < 28; iterator_bytes++)
    //   float_and_bytes.the_bytes[iterator_bytes%4] = in_message[8+iterator_bytes + DATA_PACKET_SIZE_THERAPY*iterator];
    // t[iterator].therapy_duration = float_and_bytes.the_float;

    /* Resting time */
    t[iterator].t_rest = in_message[data_start + rest_start + DATA_PACKET_SIZE_THERAPY * iterator] * 16777216 +
                         in_message[data_start + rest_start + 1 + DATA_PACKET_SIZE_THERAPY * iterator] * 65536 +
                         in_message[data_start + rest_start + 2 + DATA_PACKET_SIZE_THERAPY * iterator] * 256 +
                         in_message[data_start + rest_start + 3 + DATA_PACKET_SIZE_THERAPY * iterator];
    // for (iterator_bytes = 20; iterator_bytes < 24; iterator_bytes++)
    //   float_and_bytes.the_bytes[iterator_bytes%4] = in_message[8+iterator_bytes + DATA_PACKET_SIZE_THERAPY*iterator];
    // t[iterator].t_rest = float_and_bytes.the_float;

    /* Stimulating time */
    t[iterator].t_stim = in_message[data_start + on_start + DATA_PACKET_SIZE_THERAPY * iterator] * 16777216 +
                         in_message[data_start + on_start + 1 + DATA_PACKET_SIZE_THERAPY * iterator] * 65536 +
                         in_message[data_start + on_start + 2 + DATA_PACKET_SIZE_THERAPY * iterator] * 256 +
                         in_message[data_start + on_start + 3 + DATA_PACKET_SIZE_THERAPY * iterator];
    // for (iterator_bytes = 16; iterator_bytes < 20; iterator_bytes++)
    //   float_and_bytes.the_bytes[iterator_bytes%4] = in_message[8+iterator_bytes + DATA_PACKET_SIZE_THERAPY*iterator];
    // t[iterator].t_stim = float_and_bytes.the_float;

    /* Ramp up */
    t[iterator].t_up = in_message[data_start + rise_start + DATA_PACKET_SIZE_THERAPY * iterator] * 16777216 +
                       in_message[data_start + rise_start + 1 + DATA_PACKET_SIZE_THERAPY * iterator] * 65536 +
                       in_message[data_start + rise_start + 2 + DATA_PACKET_SIZE_THERAPY * iterator] * 256 +
                       in_message[data_start + rise_start + 3 + DATA_PACKET_SIZE_THERAPY * iterator];
    // for (iterator_bytes = 28; iterator_bytes < 32; iterator_bytes++)
    //   float_and_bytes.the_bytes[iterator_bytes%4] = in_message[8+iterator_bytes + DATA_PACKET_SIZE_THERAPY*iterator];
    // t[iterator].t_up = float_and_bytes.the_float;

    /* Ramp down */
    t[iterator].t_down = in_message[data_start + decay_start + DATA_PACKET_SIZE_THERAPY * iterator] * 16777216 +
                         in_message[data_start + decay_start + 1 + DATA_PACKET_SIZE_THERAPY * iterator] * 65536 +
                         in_message[data_start + decay_start + 2 + DATA_PACKET_SIZE_THERAPY * iterator] * 256 +
                         in_message[data_start + decay_start + 3 + DATA_PACKET_SIZE_THERAPY * iterator];
    // for (iterator_bytes = 32; iterator_bytes < 36; iterator_bytes++)
    //   float_and_bytes.the_bytes[iterator_bytes%4] = in_message[8+iterator_bytes + DATA_PACKET_SIZE_THERAPY*iterator];
    // t[iterator].t_down = float_and_bytes.the_float;

    /* Channels numbers */
    // TODO: check channel id. 1 is 1. 2 is 2.
    t[iterator].channel_id = in_message[8 + 44 + DATA_PACKET_SIZE_THERAPY * iterator] - 1;
  }

  // For debugging (no communication)
  // t[0].frequency        = 50; // Hz
  // t[0].amplitude        = 1000; // /2048 * 3.3V
  // t[0].pulse_width      = 700; // us
  // t[0].therapy_duration = 30000; // ms
  // t[0].t_rest           = 1000; // ms
  // t[0].t_stim           = 2000; // ms
  // t[0].t_up             = 500; // ms
  // t[0].t_down           = 200; // ms
  // t[0].channel_id       = 0;

  // t[1].frequency        = t[0].frequency; // Hz
  // t[1].amplitude        = 800; // /2048 * 3.3V
  // t[1].pulse_width      = 400; // us
  // t[1].therapy_duration = t[0].therapy_duration; // ms
  // t[1].t_rest           = 2000; // ms
  // t[1].t_stim           = 1000; // ms
  // t[1].t_up             = 900; // ms
  // t[1].t_down           = 500; // ms
  // t[1].channel_id       = 1;

  for (iterator = 0; iterator < num_therapy_channels; iterator++)
  {
    Serial.println("Frequency: " + String(t[iterator].frequency));
    Serial.println("Amplitude: " + String(t[iterator].amplitude));
    Serial.println("Pulse width: " + String(t[iterator].pulse_width));
    Serial.println("Total time: " + String(t[iterator].therapy_duration));
    Serial.println("Rest time: " + String(t[iterator].t_rest));
    Serial.println("Stim time: " + String(t[iterator].t_stim));
    Serial.println("Up ramp: " + String(t[iterator].t_up));
    Serial.println("Down ramp: " + String(t[iterator].t_down));
    Serial.println("Channel ID: " + String(t[iterator].channel_id));
    Serial.println("----------------");
  }

  Serial.print("Therapy: ");
  if (in_message[data_start + auto_start + 3] == COMM_COMMAND_SUBTYPE_OPEN_LOOP)
  {
    Serial.println("Open Loop");
    therapy_type = open_loop;
    leave_loop = true;
    return 0;
  }
  else if (in_message[data_start + auto_start + 3] == COMM_COMMAND_SUBTYPE_CLOSED_LOOP)
  {
    Serial.println("Closed Loop");
    therapy_type = closed_loop;
    leave_loop = true;
    return 0;
  }
  else
    therapy_type = no_therapy;

  return -1;
}

boolean startDiagnosisResponse(boolean verbose)
{
  byte* response;
  int response_length;

  byte* unsupported_channels;
  byte num_unsupported_channels = 0;
  int iterator, iterator_bytes;

  // unsupported_channels = (byte*) malloc(0*sizeof(byte));

  // // Finding which channels are unsupported
  // for (iterator = 0; iterator < d.num_channels; iterator++)
  // {
  //   if (d.channels[iterator] < 0 || d.channels[iterator] > 3)
  //   {
  //     unsupported_channels = (byte*) realloc(unsupported_channels, sizeof(byte));
  //     unsupported_channels[num_unsupported_channels] = iterator;
  //     num_unsupported_channels++;
  //   }
  // }

  // Assemblying response
  response_length = 11;

  response = (byte*) malloc(response_length * sizeof(byte));
  response[0] = COMM_RESPONSE_START_BYTE;
  if (null_id)
    for (iterator_bytes = 0; iterator_bytes < 4; iterator_bytes++)
      response[1 + iterator_bytes] = 0;
  else
    for (iterator_bytes = 0; iterator_bytes < 4; iterator_bytes++)
      response[1 + iterator_bytes] = message_id[iterator_bytes];

  if (diagnosis_type == rheobase) {
    response[5] = COMM_COMMAND_SUBTYPE_RHEOBASE;
  } else if (diagnosis_type == chronaxie) {
    response[5] = COMM_COMMAND_SUBTYPE_CHRONAXIE;
  } else if (diagnosis_type == accommodation) {
    response[5] = COMM_COMMAND_SUBTYPE_ACCOMODATION;
  }

  response[6] = 0;
  //response[5] = COMM_COMMAND_MESSAGE_DIAGNOSIS;
  //float_and_bytes.the_float = 1 + num_unsupported_channels;
  response[7] = 1;
  response[8] = COMM_RESPONSE_RESULT_OK;

  // Checksum
  response[9] = 0;
  for (iterator = 1; iterator < 4; iterator++)
    response[9] += response[iterator];

  response[10] = COMM_END_BYTE;

  // Sending response
  sendBTData(response, response_length, verbose);

  return true;

  // if (num_unsupported_channels)
  // {
  //   response = (byte*) malloc(response_length*sizeof(byte));
  //    response = (byte*) malloc(response_length*sizeof(byte));
  //   response[0] = COMM_RESPONSE_START_BYTE;
  //   if (null_id)
  //     for (iterator_bytes = 0; iterator_bytes < 4; iterator_bytes++)
  //       response[1+iterator_bytes] = 0;
  //   else
  //     for (iterator_bytes = 0; iterator_bytes < 4; iterator_bytes++)
  //       response[1+iterator_bytes] = message_id[iterator_bytes];
  //   response[5] = COMM_COMMAND_MESSAGE_DIAGNOSIS;
  //   float_and_bytes.the_float = 1 + num_unsupported_channels;
  //   response[6] = float_and_bytes.the_bytes[1];
  //   response[7] = float_and_bytes.the_bytes[0];
  //   response[8] = COMM_RESPONSE_RESULT_UNSUPPORTED_CHANNELS;

  //   // Unsupported channels listing
  //   for (iterator = 0; iterator < num_unsupported_channels; iterator++)
  //     response[9+iterator] = unsupported_channels[iterator];

  //   // Checksum
  //   response[9+num_unsupported_channels] = 0;
  //   for (iterator = 1; iterator < 3 + num_unsupported_channels; iterator++)
  //     response[9+num_unsupported_channels] += response[iterator];

  //   response[9+num_unsupported_channels+1] = COMM_END_BYTE;

  //   // Sending response
  //   sendBTData(response, response_length, verbose);

  //   return false;
  // }
  // else
  // {
  //   response = (byte*) malloc(response_length*sizeof(byte));
  //   response[0] = COMM_RESPONSE_START_BYTE;
  //   if (null_id)
  //     for (iterator_bytes = 0; iterator_bytes < 4; iterator_bytes++)
  //     response[1+iterator_bytes] = 0;
  //   else
  //     for (iterator_bytes = 0; iterator_bytes < 4; iterator_bytes++)
  //       response[1+iterator_bytes] = message_id[iterator_bytes];
  //   response[5] = COMM_COMMAND_MESSAGE_DIAGNOSIS;
  //   float_and_bytes.the_float = 1 + num_unsupported_channels;
  //   response[6] = COMM_RESPONSE_RESULT_OK;

  //   // Checksum
  //   response[7] = 0;
  //   for (iterator = 1; iterator < 4; iterator++)
  //     response[7] += response[iterator];

  //   response[8] = COMM_END_BYTE;

  //   // Sending response
  //   sendBTData(response, response_length, verbose);

  //   return true;
  // }
}

boolean startTherapyResponse(boolean verbose)
{
  byte* response;
  int response_length;

  byte* unsupported_channels;
  byte num_unsupported_channels = 0;

  int iterator, iterator_bytes;

  unsupported_channels = (byte*) malloc(0 * sizeof(byte));

  // Finding which channels are unsupported
  for (iterator = 0; iterator < num_therapy_channels; iterator++)
  {
    if (t[iterator].channel_id < 0 || t[iterator].channel_id > 3)
    {
      unsupported_channels = (byte*) realloc(unsupported_channels, sizeof(byte));
      unsupported_channels[num_unsupported_channels] = iterator;
      num_unsupported_channels++;
    }
  }

  Serial.println("Number of unsupported channels: " + String(num_unsupported_channels));

  // Assemblying response
  response_length = 11 + num_unsupported_channels;
  if (num_unsupported_channels)
  {
    response = (byte*) malloc(response_length * sizeof(byte));
    response[0] = COMM_RESPONSE_START_BYTE;
    if (null_id)
      for (iterator_bytes = 0; iterator_bytes < 4; iterator_bytes++)
        response[1 + iterator_bytes] = 0;
    else
      for (iterator_bytes = 0; iterator_bytes < 4; iterator_bytes++)
        response[1 + iterator_bytes] = message_id[iterator_bytes];
    response[5] = COMM_COMMAND_MESSAGE_THERAPY;
    response[6] = 0;
    response[7] = num_unsupported_channels + 1;
    response[8] = COMM_RESPONSE_RESULT_UNSUPPORTED_CHANNELS;

    // Unsupported channels listing
    for (iterator = 0; iterator < num_unsupported_channels; iterator++)
      response[9 + iterator] = unsupported_channels[iterator];

    // Checksum
    response[9 + num_unsupported_channels] = 0;
    for (iterator = 1; iterator < 3 + num_unsupported_channels; iterator++)
      response[9 + num_unsupported_channels] += response[iterator];

    response[9 + num_unsupported_channels + 1] = COMM_END_BYTE;

    // Sending response
    sendBTData(response, response_length, verbose);

    return false;
  }
  else
  {
    response = (byte*) malloc(response_length * sizeof(byte));
    response[0] = COMM_RESPONSE_START_BYTE;
    if (null_id)
      for (iterator_bytes = 0; iterator_bytes < 4; iterator_bytes++)
        response[1 + iterator_bytes] = 0;
    else
      for (iterator_bytes = 0; iterator_bytes < 4; iterator_bytes++)
        response[1 + iterator_bytes] = message_id[iterator_bytes];
    response[5] = COMM_COMMAND_MESSAGE_THERAPY;
    response[6] = 0;
    response[7] = 1;
    response[8] = COMM_RESPONSE_RESULT_OK;

    // Checksum
    response[9] = 0;
    for (iterator = 1; iterator < 9; iterator++)
      response[9] += response[iterator];

    response[10] = COMM_END_BYTE;

    // Sending response
    sendBTData(response, response_length, verbose);

    return true;
  }
}

int changeParametersRT()
{
  int iterator, iterator_bytes, updated_channel_id, updateted_channel_iterator, updated_param_id, updated_param_value;
  int data_start = 8;
  int data_value_start = 2;
  byte* updated_param_bytes;
  byte* update_message_id;

  if (!Serial1.available())
    return -1;

  in_message = readBTMessage(true, MAX_PACKET_SIZE);

  // Is it an update message?
  if (in_message[5] == COMM_COMMAND_MESSAGE_THERAPY_UPDATE) {

    if (in_message[7] < DATA_PACKET_SIZE_THERAPY_UPDATE)
      return -1; // data length must be 1 (therapy type) + (32+1)*num_channels

    updated_param_bytes = (byte*) malloc(4 * sizeof(byte));
    update_message_id = (byte*) malloc(4 * sizeof(byte));

    /* Printing income message */
    for (iterator = 0; iterator < MAX_PACKET_SIZE; iterator++)
    {
      Serial.print(in_message[iterator], HEX);
      Serial.print(" ");
    }
    Serial.println();
    Serial.println("Atualizacao de parametros");

    /* Updating values */
    updated_channel_id = in_message[data_start] - 1;
    for (int iterator_active_channels = 0; iterator_active_channels <= num_therapy_channels; iterator_active_channels++) {
      if (updated_channel_id == t[iterator_active_channels].channel_id) {
        updateted_channel_iterator = iterator_active_channels;
        Serial.println(updateted_channel_iterator);
        break;
      }
      if (iterator_active_channels == num_therapy_channels)
        Serial.println("Channel not supported");
    }
    updated_param_id = in_message[data_start + 1];
    updated_param_value = in_message[data_start + data_value_start] * 16777216 +
                          in_message[data_start + data_value_start + 1] * 65536 +
                          in_message[data_start + data_value_start + 2] * 256 +
                          in_message[data_start + data_value_start + 3];

    updated_param_bytes[0] = in_message[data_start + data_value_start];
    updated_param_bytes[1] = in_message[data_start + data_value_start + 1];
    updated_param_bytes[2] = in_message[data_start + data_value_start + 2];
    updated_param_bytes[3] = in_message[data_start + data_value_start + 3];

    update_message_id[0] = in_message[1];
    update_message_id[1] = in_message[2];
    update_message_id[2] = in_message[3];
    update_message_id[3] = in_message[4];

    Serial.print("Canal ");
    Serial.print(updated_channel_id);
    Serial.print(", ");
    switch (updated_param_id) {
      case 1: //intensity
        t[updateted_channel_iterator].amplitude = updated_param_value * 20.47 / 1000;
        Serial.print("amplitude ");
        Serial.println(t[0].amplitude / 20.47);
        break;
      case 2: //frequency
        t[updateted_channel_iterator].frequency = updated_param_value;
        Serial.print("frequencia ");
        break;
      case 3: //pulse_width
        t[updateted_channel_iterator].pulse_width = updated_param_value;
        Serial.print("pulse width ");
        break;
      case 4: //t_on
        t[updateted_channel_iterator].t_stim = updated_param_value;
        Serial.print("t_on ");
        break;
      case 5: //t_off
        t[updateted_channel_iterator].t_rest = updated_param_value;
        Serial.print("t_off ");
        break;
      case 6: //rise
        t[updateted_channel_iterator].t_up = updated_param_value;
        Serial.print("rampa de subida ");
        break;
      case 7: //decay
        t[updateted_channel_iterator].t_down = updated_param_value;
        Serial.print("rampa de descida ");
        break;
      default:
        break;
    }


    Serial.println(updated_param_value);

    changeParamsResponse(true, updated_param_bytes, update_message_id);
    return 0;

    /* Printing income message */
    for (iterator = 0; iterator < 80; iterator++)
    {
      Serial.print(in_message[iterator], HEX);
      Serial.print(" ");
    }
  }
  // Is it a finish operation messagem?
  else if (in_message[5] == COMM_RT_STOP_ALL) {
    stop_therapy = true;
    zeroChannels();
    Serial.println("Operacao interrompida");
    finishOperationResponse(true);
    return 0;
  }
  else
    // Its none of the above
    return -1;
}

void streamDiagnosisMessage(boolean verbose)
{
  byte* message;
  int message_length;

  int iterator;

  message_length = (10 + d.num_channels * 4);
  message = (byte*) malloc(message_length * sizeof(byte));

  // Assemblying message
  message[0] = COMM_NOTIFICATION_START_BYTE;
  //message[0] = 0x25;
  //message[1] = COMM_STREAM_SUBTYPE_DIAGOSIS;
  //message[2] = 2+d.num_channels*6;

  message[1] = 0;
  message[2] = 0;
  message[3] = 0;
  message[4] = 0;


  if (diagnosis_type == chronaxie)
    message[5] = COMM_COMMAND_SUBTYPE_CHRONAXIE;
  else if (diagnosis_type == rheobase)
    message[5] = COMM_COMMAND_SUBTYPE_RHEOBASE;
  else if (diagnosis_type == accommodation)
    message[5] = COMM_COMMAND_SUBTYPE_ACCOMODATION;
  else if (diagnosis_type == science)
    message[5] = COMM_COMMAND_SUBTYPE_SCIENCE;

  //message[3] = COMM_STREAM_STATUS_FINISHED;m
  message[6] = 0;
  message[7] = 4;

  // message[8] = 0;
  // message[9] = 0;
  // message[10] = 7;
  // message[11] = 208;


  //converter de entero para Byte
  message[8] = (int)d.measured_data[0] / 16777216;
  message[9] = ((int)d.measured_data[0] % 16777216) / 65536;
  message[10] = ((int)d.measured_data[0] % 65536) / 256;
  message[11] = (int)d.measured_data[0] % 256;

  // for (iterator = 0; iterator < d.num_channels; iterator++)
  // {
  //   message[5+(iterator*6)] = d.channels[iterator]+1;
  //   message[6+(iterator*6)] = COMM_STREAM_STATUS_FINISHED; // check before if channel is ok (not yet supported)

  //   float_and_bytes.the_float = d.measured_data[iterator];

  //   message[7+(iterator*6)]   = float_and_bytes.the_bytes[3];
  //   message[8+(iterator*6)]   = float_and_bytes.the_bytes[2];
  //   message[9+(iterator*6)]   = float_and_bytes.the_bytes[1];
  //   message[10+(iterator*6)]  = float_and_bytes.the_bytes[0];
  // }

  //checksum
  message[12] = 0;
  for (iterator = 1; iterator < 12; iterator++)
    message[12] += message[iterator];
  //message[12] = 0xD4;


  message[13] = COMM_END_BYTE;

  // Sending message
  //sendBTData(message, message_length, verbose);
  // byte* message;
  // int message_length;

  // int iterator;

  // message_length = (7+d.num_channels*6);
  // message = (byte*) malloc(message_length*sizeof(byte));

  // // Assemblying message
  // message[0] = COMM_NOTIFICATION_START_BYTE;
  // message[1] = COMM_STREAM_SUBTYPE_DIAGOSIS;
  // message[2] = 2+d.num_channels*6;

  // if (diagnosis_type == chronaxie)
  //   message[3] = COMM_COMMAND_SUBTYPE_CHRONAXIE;
  // else if (diagnosis_type == rheobase)
  //   message[3] = COMM_COMMAND_SUBTYPE_RHEOBASE;
  // else if (diagnosis_type == accommodation)
  //   message[3] = COMM_COMMAND_SUBTYPE_ACCOMODATION;
  // else if (diagnosis_type == science)
  //   message[3] = COMM_COMMAND_SUBTYPE_SCIENCE;

  // message[4] = COMM_STREAM_STATUS_STIMULATING;

  // for (iterator = 0; iterator < d.num_channels; iterator++)
  // {
  //   message[5+(iterator*6)] = d.channels[iterator] + 1;
  //   message[6+(iterator*6)] = COMM_RESPONSE_RESULT_OK; // check before if channel is ok (not yet supported)

  //   float_and_bytes.the_float = d.measured_data[iterator];

  //   message[7+(iterator*6)]   = float_and_bytes.the_bytes[3];
  //   message[8+(iterator*6)]   = float_and_bytes.the_bytes[2];
  //   message[9+(iterator*6)]   = float_and_bytes.the_bytes[1];
  //   message[10+(iterator*6)]  = float_and_bytes.the_bytes[0];
  // }

  // // checksum
  // message[5+(d.num_channels*6)] = 0;
  // for (iterator = 1; iterator < 5+(d.num_channels*6); iterator++)
  //   message[5+(d.num_channels*6)] += message[iterator];

  // message[5+(d.num_channels*6)+1] = COMM_END_BYTE;

  // Sending message
  sendBTData(message, message_length, verbose);
}

void streamTherapyMessage(boolean verbose)
{
  byte* message;
  int message_length;
  int data_start = 8;
  int data_size_channel = 9;
  int uAmpere;
  int iterator;

  message_length = (10 + num_therapy_channels * 9);
  message = (byte*) malloc(message_length * sizeof(byte));

  // Assemblying message
  message[0] = COMM_NOTIFICATION_START_BYTE;
  for (int iterator_bytes = 0; iterator_bytes < 4; iterator_bytes++)
    message[1 + iterator_bytes] = 0;
  message[5] = COMM_COMMAND_MESSAGE_STREAM;
  message[6] = 0;
  message[7] = num_therapy_channels * data_size_channel;

  // if (therapy_type == open_loop)
  //   message[3] = COMM_COMMAND_SUBTYPE_OPEN_LOOP;
  // else if (therapy_type == closed_loop)
  //   message[3] = COMM_COMMAND_SUBTYPE_CLOSED_LOOP;

  // message[4] = COMM_STREAM_STATUS_STIMULATING;

  for (iterator = 0; iterator < num_therapy_channels; iterator++)
  {
    // TODO: Check channels id
    message[data_start + data_size_channel * iterator] = t[iterator].channel_id + 1;

    // TODO: Improve this data and add movement data
    message[data_start + data_size_channel * iterator + 1] = 0;
    message[data_start + data_size_channel * iterator + 2] = 0;
    message[data_start + data_size_channel * iterator + 3] = 0;
    message[data_start + data_size_channel * iterator + 4] = 0;

    uAmpere = int(t[iterator].amplitude / 20.47 * 1000);
    message[data_start + data_size_channel * iterator + 5] = uAmpere / 16777216;
    message[data_start + data_size_channel * iterator + 6] = (uAmpere % 16777216) / 65536;
    message[data_start + data_size_channel * iterator + 7] = (uAmpere % 65536) / 256;
    message[data_start + data_size_channel * iterator + 8] = uAmpere % 256;
  }

  // checksum
  message[data_start + data_size_channel * num_therapy_channels] = 0;
  for (iterator = 1; iterator < data_start + data_size_channel * num_therapy_channels; iterator++)
    message[data_start + data_size_channel * num_therapy_channels] += message[iterator];

  message[data_start + data_size_channel * num_therapy_channels + 1] = COMM_END_BYTE;

  // Sending message
  sendBTData(message, message_length, verbose);
}

void sendBTData(byte* out, int len, boolean verbose)
{
  Serial1.write(0); //retificaçao da mensagem no app parea evitar o quebre de linha
  for (int i = 0; i < len; i++)
    Serial1.write(out[i]);

  if (verbose)
  {
    Serial.print("To BT(" + String(len) + "): ");
    //Serial.print(0,HEX);
    for (int iterator = 0; iterator < len; iterator++)
    {
      Serial.print(out[iterator], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void finishCommunication()
{
  if (session_type == diagnosis)
    streamDiagnosisFinishMessage(true);
  else if (session_type == therapy)
    streamTherapyFinishMessage(true);

  // TODO: study whether to remove this or not
  if (getFinishCommand()) // TODO: maybe change to getFinishResponse()
  {
    Serial.println("Finish command received!");
    sendFinishResponse(true); // TODO: maybe just delete this
  }
}

void streamDiagnosisFinishMessage(boolean verbose)
{
  byte* message;
  int message_length;

  int iterator;

  // colocar aqui sinal de que o diagnostico acabou (low)
  digitalWrite(REC_SYNC_PIN, LOW); //encierra la gravacion de datos

  message_length = (10 + d.num_channels * 4);
  message = (byte*) malloc(message_length * sizeof(byte));

  // Assemblying message
  message[0] = COMM_NOTIFICATION_START_BYTE;
  //message[0] = 0x25;
  //message[1] = COMM_STREAM_SUBTYPE_DIAGOSIS;
  //message[2] = 2+d.num_channels*6;

  message[1] = 0;
  message[2] = 0;
  message[3] = 0;
  message[4] = 0;


  if (diagnosis_type == chronaxie)
    message[5] = COMM_COMMAND_SUBTYPE_CHRONAXIE;
  else if (diagnosis_type == rheobase)
    message[5] = COMM_COMMAND_SUBTYPE_RHEOBASE;
  else if (diagnosis_type == accommodation)
    message[5] = COMM_COMMAND_SUBTYPE_ACCOMODATION;
  else if (diagnosis_type == science)
    message[5] = COMM_COMMAND_SUBTYPE_SCIENCE;

  //message[3] = COMM_STREAM_STATUS_FINISHED;m
  message[6] = 0;
  message[7] = 4;

  message[8] = 0;
  message[9] = 0;
  message[10] = 7;
  message[11] = 208;


  // //converter de entero para Byte
  message[8] = (int)d.measured_data[0] / 16777216;
  message[9] = ((int)d.measured_data[0] % 16777216) / 65536;
  message[10] = ((int)d.measured_data[0] % 65536) / 256;
  message[11] = (int)d.measured_data[0] % 256;

  // for (iterator = 0; iterator < d.num_channels; iterator++)
  // {
  //   message[5+(iterator*6)] = d.channels[iterator]+1;
  //   message[6+(iterator*6)] = COMM_STREAM_STATUS_FINISHED; // check before if channel is ok (not yet supported)

  //   float_and_bytes.the_float = d.measured_data[iterator];

  //   message[7+(iterator*6)]   = float_and_bytes.the_bytes[3];
  //   message[8+(iterator*6)]   = float_and_bytes.the_bytes[2];
  //   message[9+(iterator*6)]   = float_and_bytes.the_bytes[1];
  //   message[10+(iterator*6)]  = float_and_bytes.the_bytes[0];
  // }

  // checksum
  message[12] = 0;
  for (iterator = 1; iterator < 12; iterator++)
    message[12] += message[iterator];
  //message[12] = 0xD4;


  message[13] = COMM_END_BYTE;

  // Sending message
  sendBTData(message, message_length, verbose);
}

void streamTherapyFinishMessage(boolean verbose)
{
  byte* message;
  int message_length;
  int data_start = 8;
  int data_size_channel = 9;

  int iterator;

  message_length = (11);
  message = (byte*) malloc(message_length * sizeof(byte));

  // Assemblying message
  message[0] = COMM_NOTIFICATION_START_BYTE;
  for (int iterator_bytes = 0; iterator_bytes < 4; iterator_bytes++)
    message[1 + iterator_bytes] = 0;
  message[5] = COMM_COMMAND_MESSAGE_STREAM_FINISH;
  message[6] = 0;
  message[7] = 1;
  message[8] = 0; // If ok

  // checksum
  message[9] = 0;
  for (iterator = 1; iterator < 9; iterator++)
    message[9] += message[iterator];

  message[10] = COMM_END_BYTE;

  // Sending message
  sendBTData(message, message_length, verbose);

  // byte* message;
  // int message_length;

  // int iterator;

  // message_length = (7+num_therapy_channels*6);
  // message = (byte*) malloc(message_length*sizeof(byte));

  // // Assemblying message
  // message[0] = COMM_NOTIFICATION_START_BYTE;
  // message[1] = COMM_STREAM_SUBTYPE_THERAPY;
  // message[2] = 2+num_therapy_channels*6;

  // if (therapy_type == open_loop)
  //   message[3] = COMM_COMMAND_SUBTYPE_OPEN_LOOP;
  // else if (therapy_type == closed_loop)
  //   message[3] = COMM_COMMAND_SUBTYPE_CLOSED_LOOP;

  // message[4] = COMM_STREAM_STATUS_FINISHED;

  // for (iterator = 0; iterator < num_therapy_channels; iterator++)
  // {
  //   message[5+(iterator*6)] = t[iterator].channel_id+1;
  //   message[6+(iterator*6)] = COMM_STREAM_STATUS_FINISHED; // check before if channel is ok (not yet supported)

  //   float_and_bytes.the_float = t[iterator].measured_data;

  //   message[7+(iterator*6)]   = float_and_bytes.the_bytes[3];
  //   message[8+(iterator*6)]   = float_and_bytes.the_bytes[2];
  //   message[9+(iterator*6)]   = float_and_bytes.the_bytes[1];
  //   message[10+(iterator*6)]  = float_and_bytes.the_bytes[0];
  // }

  // // checksum
  // message[5+(num_therapy_channels*6)] = 0;
  // for (iterator = 1; iterator < 5+(num_therapy_channels*6); iterator++)
  //   message[5+(num_therapy_channels*6)] += message[iterator];

  // message[5+(num_therapy_channels*6)+1] = COMM_END_BYTE;

  // // Sending message
  // sendBTData(message, message_length, verbose);
}

boolean getFinishCommand()
{
  byte* finish_message;
  finish_message = readBTMessage(true, MAX_PACKET_SIZE);

  if (finish_message[5] == COMM_RT_STOP_ALL)
    return true;

  return false;
}

void sendFinishResponse(boolean verbose)
{
  byte* response;
  int response_length;
  int iterator, iterator_bytes;

  // Assemblying response
  response_length = 15;
  response = (byte*) malloc(response_length * sizeof(byte));
  response[0] = COMM_RESPONSE_START_BYTE;
  if (null_id)
    for (iterator_bytes = 0; iterator_bytes < 4; iterator_bytes++)
      response[1 + iterator_bytes] = 0;
  else
    for (iterator_bytes = 0; iterator_bytes < 4; iterator_bytes++)
      response[1 + iterator_bytes] = message_id[iterator_bytes];
  response[5] = COMM_RT_STOP_ALL;
  response[6] = 0;
  response[7] = 1;
  response[8] = COMM_RESPONSE_RESULT_OK;

  // Checksum
  response[9] = 0;
  for (iterator = 1; iterator < 9; iterator++)
    response[9] += response[iterator];

  response[10] = COMM_END_BYTE;

  // Sending response
  sendBTData(response, response_length, verbose);
}

/* Stimulation functions */
void diagnosisStimPulse(float max_amplitude, float pulse_width)
{

  Serial.print("Diagnosis Type: ");//Tipo2 - Reobase || Tipo 1 - Cronaxia || Tipo 3 - Acomodacion
  Serial.println(diagnosis_type);

  uint16_t stim_value;

  diagnosis_old_status = diagnosis_no_pulse;
  diagnosis_curr_status = first_pulse;

  while (diagnosis_old_status != inter_pulse)
  {
    if (diagnosis_old_status != diagnosis_curr_status)
    {
      t_this_status = micros();
      diagnosis_old_status = diagnosis_curr_status;
    }

    /* First pulse */
    if (diagnosis_curr_status == first_pulse)
    {
      t_now = micros();

      //inicio del pulso
      digitalWrite(STIM_PIN, HIGH); //Inicia la gravacion de datos

      while ((t_now - t_this_status) < pulse_width)
      {
        stim_value = getCurrentAmplitudeValue((t_now - t_this_status), POSITIVE_POLARITY, max_amplitude, pulse_width);
        // Serial.println (stim_value );
        sendStimValue(d.channels[0], DAC_OP_MODE, stim_value);

        //teste de cronaxia sin depeccion de movimiento para verificar tiempo de la largura de pulso mínima
        // if (diagnosis_type != 1) {
        //   if (!d.is_contracted)
        //     checkContraction();
        // }
        if (!d.is_contracted)
           checkContraction();
        t_now = micros();
      }

      //final del pulso
      //digitalWrite(STIM_PIN, LOW); //encierra la gravacion de dato

      sendStimValue(d.channels[0], DAC_OP_MODE, (uint16_t)(STIM_ZERO));

      //diagnosis_curr_status = inter_pulse; //habilita onda monopolar
      diagnosis_curr_status = second_pulse; //habilita onda bipolar

      //Serial.println ("Valor Primer pulso positivo" );
    }
    /* Interphase */
    else if (diagnosis_curr_status == inter_phase)
    {
      t_now = micros();
      while ((t_now - t_this_status) < INTERPHASE_DELAY)
      {
        // TODO: check if it is necessary to check contraction when not in first pulse
        if (!d.is_contracted)
          checkContraction();
        t_now = micros();
      }

      diagnosis_curr_status = second_pulse;
      //Serial.println ("interpulso 1 Monopolar" );
    }

    /* Second pulse */
    else if (diagnosis_curr_status == second_pulse)
    {
      t_now = micros();

      //inicio del pulso
      //digitalWrite(STIM_PIN, HIGH); //encierra la gravacion de datos

      while ((t_now - t_this_status) < pulse_width)
      {
        stim_value = getCurrentAmplitudeValue((t_now - t_this_status), NEGATIVE_POLARITY, max_amplitude, pulse_width);
        sendStimValue(d.channels[0], DAC_OP_MODE, stim_value);

        //teste de cronaxia sin depeccion de movimiento para verificar tiempo de la largura de pulso mínima
        // if (diagnosis_type != 1) {
        //   if (!d.is_contracted)
        //     checkContraction();
        // }

        if (!d.is_contracted)
            checkContraction();
        t_now = micros();
      }

      //final del pulso
      digitalWrite(STIM_PIN, LOW); //encierra la gravacion de dato

      sendStimValue(d.channels[0], DAC_OP_MODE, (uint16_t)STIM_ZERO);

      diagnosis_curr_status = inter_pulse;

      // Serial.println ("Segundo pulso negativo" );

    }

    /* Interpulse */
    else if (diagnosis_curr_status == inter_pulse)
    {
      ///// para verificar la largura de pulso en cronaxia
      // int valorPulso = 0;
      // if (diagnosis_type == 1) { //para modificar el valor delpulso en chronaxie
      //   valorPulso = 10000;
      // } else {
      //   valorPulso = INTERPULSE_DELAY;
      // }

      t_now = micros();
      while ((t_now - t_this_status) < INTERPULSE_DELAY)//Original
      // while ((t_now - t_this_status) < valorPulso)
      {

        //teste de cronaxia sin depeccion de movimiento para verificar tiempo de la largura de pulso mínima
        // if (diagnosis_type != 1) {
        //   if (!d.is_contracted)
        //     checkContraction();
        // }

        if (!d.is_contracted)
           checkContraction();
        t_now = micros();
      }

      diagnosis_curr_status = first_pulse;

      //Serial.println ("interpulso 2 bipolar" );
    }
  }//fin while inicio
}

void openLoopStimPulse()
{
  uint16_t* stim_value = (uint16_t*) malloc(num_therapy_channels * sizeof(uint16_t));

  int iterator;
  long data_sending_time;
  long processing_time;
  stop_therapy = false;

  /* Initializing */
  for (iterator = 0; iterator < num_therapy_channels; iterator++)
  {
    t[iterator].macro_state = min_amp;
    t[iterator].T_total = (long) t[iterator].t_rest + t[iterator].t_up + t[iterator].t_stim + t[iterator].t_down;
    t[iterator].t_total = 1 / t[iterator].frequency * 1000000;

    /* Send load to charge */
    switch (t[iterator].channel_id)
    {
      case 0:
        digitalWrite(RELAY_CH_1, HIGH);
        break;
      case 1:
        digitalWrite(RELAY_CH_2, HIGH);
        break;
      case 2:
        digitalWrite(RELAY_CH_3, HIGH);
        break;
      case 3:
        digitalWrite(RELAY_CH_4, HIGH);
        break;
      default:
        break;
    }
  }

  /* Main stim loop */
  t_begin = millis();
  data_sending_time = millis();
  while ((t_now - t_begin) < t[0].therapy_duration && !stop_therapy)
  {
    processing_time = micros();

    /* Find in which macro_state each channel is */
    for (iterator = 0; iterator < num_therapy_channels; iterator++)
    {
      t[iterator].t_now = t_now % t[iterator].T_total;
      t[iterator].macro_state = getMacroState(t[iterator].t_now, iterator);

      /* Get current stim_value for each channel */
      switch (t[iterator].macro_state)
      {
        case min_amp:
          stim_value[iterator] =  0;
          break;
        case ramp_up:
          stim_value[iterator] =  (t[iterator].amplitude / t[iterator].t_up) * (t[iterator].t_now - t[iterator].t_rest);
          break;
        case max_amp:
          stim_value[iterator] =  t[iterator].amplitude;
          break;
        case ramp_down:
          stim_value[iterator] =  t[iterator].amplitude -
                                  ((t[iterator].amplitude / t[iterator].t_down) * (t[iterator].t_now - (t[iterator].t_rest + t[iterator].t_up + t[iterator].t_stim)));
          break;
        default:
          stim_value[iterator] = 0;
          return;
      }
    }

    /* Send data to app every second */
    if (millis() - data_sending_time > 1000)
    {
      for (iterator = 0; iterator < num_therapy_channels; iterator++)
        t[iterator].measured_data = getFilteredSignal(ACCEL_PIN(iterator)); // TODO: send amplitude instead

      streamTherapyMessage(true);

      data_sending_time = millis();
    }

    /* Execute micro_state */

    // Check stop button
    if (buttonStop())
    {
      stop_therapy = true;
      zeroChannels();
      Serial.println("Operacao interrompida por botão.");
      finishOperationResponse(true);
      continue;
    }

    for (iterator = 0; iterator < num_therapy_channels; iterator++)
    {

      sendStimValue(t[iterator].channel_id, DAC_OP_MODE, (uint16_t) (STIM_ZERO + stim_value[iterator]));
      // debug
      // Serial1.print("Positivo: ");
      // Serial1.println((uint16_t) (STIM_ZERO + stim_value[iterator]));
      delayMicroseconds(t[iterator].pulse_width);
      sendStimValue(t[iterator].channel_id, DAC_OP_MODE, (uint16_t) (STIM_ZERO - stim_value[iterator]));
      // debug
      // Serial1.print("Negativo: ");
      // Serial1.println((uint16_t) (STIM_ZERO - stim_value[iterator]));
      delayMicroseconds(t[iterator].pulse_width);
      // Serial1.print("STIM_ZERO: ");
      // Serial1.println(STIM_ZERO);
      // Serial1.print("stim_value: ");
      // Serial1.println(stim_value[iterator]);

      sendStimValue(t[iterator].channel_id, DAC_OP_MODE, (uint16_t) (STIM_ZERO));

      delayMicroseconds(INTERCHANNEL_DELAY);
    }

    /* Realtime parameter changing */
    if (!changeParametersRT())
      continue;

    /* Final delay for frequency */
    processing_time = micros() - processing_time;
    long delay_time = t[0].t_total - processing_time;
    if (delay_time > 0)
      delayMicroseconds(delay_time);

    t_now = millis();
  }
}

therapy_macro_states_t getMacroState(long sometime, int which_channel)
{
  if (sometime < t[which_channel].t_rest)
    return min_amp;
  else if (sometime < (t[which_channel].t_rest + t[which_channel].t_up))
    return ramp_up;
  else if (sometime < (t[which_channel].t_rest + t[which_channel].t_up + t[which_channel].t_stim))
    return max_amp;
  else if (sometime < (t[which_channel].t_rest + t[which_channel].t_up + t[which_channel].t_stim + t[which_channel].t_down))
    return ramp_down;
  else
    return therapy_no_pulse;
}

void closedLoopStimPulse(control_type_t control_type)
{
  int iterator;

  uint16_t* stim_value = (uint16_t*) malloc(num_therapy_channels * sizeof(uint16_t));
  circular_buffer = (float*) malloc(num_therapy_channels * BUFFER_SIZE * sizeof(float));
  buffer_iterator = (int*) malloc(num_therapy_channels * sizeof(int));

  long processing_time;

  float control_p = 0;
  float control_i = 0;
  float control_d = 0;
  float DT = 1 / t[0].frequency;
  boolean start_control = false;

  /* Initializing */
  for (iterator = 0; iterator < num_therapy_channels; iterator++)
  {
    t[iterator].control_factor = 1;
    t[iterator].macro_state = min_amp;
    t[iterator].T_total = (long) t[iterator].t_rest + t[iterator].t_up + t[iterator].t_stim + t[iterator].t_down;
    t[iterator].t_total = 1 / t[iterator].frequency * 1000000;
  }
  initControlParameters();

  /* Main stim loop */
  t_begin = millis();
  while ((t_now - t_begin) < t[0].therapy_duration)
  {
    processing_time = micros();

    /* Find in which macro_state each channel is */
    for (iterator = 0; iterator < num_therapy_channels; iterator++)
    {
      t[iterator].t_now = t_now % t[iterator].T_total;
      t[iterator].macro_state = getMacroState(t[iterator].t_now, iterator);

      /* Initiate control algorithm (gets in only once) */
      if (!start_control && (t_now - t_begin) > (t[iterator].t_up + 1 / t[iterator].frequency * BUFFER_SIZE))
      {
        start_control = true;
        t[iterator].reference_data = getRMSAcceleration(iterator); // TODO: change this reference
      }
      else
        t[iterator].measured_data = getRMSAcceleration(iterator); // adding samples to buffer

      /* Apply control algorithm */
      if (start_control && t[iterator].macro_state == max_amp)
        t[iterator].control_factor = controlAlgorithm(control_type, iterator, control_p, control_i, control_d, DT);
      else
        t[iterator].control_factor = 1;

      /* Get current stim_value for each channel */
      switch (t[iterator].macro_state)
      {
        case min_amp:
          stim_value[iterator] =  t[iterator].control_factor * 0;
          break;
        case ramp_up:
          stim_value[iterator] =  t[iterator].control_factor * (t[iterator].amplitude / t[iterator].t_up) * (t[iterator].t_now - t[iterator].t_rest);
          break;
        case max_amp:
          stim_value[iterator] =  t[iterator].control_factor * t[iterator].amplitude;
          break;
        case ramp_down:
          stim_value[iterator] =  t[iterator].control_factor * t[iterator].amplitude -
                                  ((t[iterator].amplitude / t[iterator].t_down) * (t[iterator].t_now
                                      - (t[iterator].t_rest + t[iterator].t_up + t[iterator].t_stim)));
          break;
        default:
          stim_value[iterator] =  t[iterator].control_factor * 0;
          return;
      }
    }

    /* Execute micro_state */
    for (iterator = 0; iterator < num_therapy_channels; iterator++)
    {
      sendStimValue(t[iterator].channel_id, DAC_OP_MODE, (uint16_t) (STIM_ZERO + stim_value[iterator]));
      delayMicroseconds(t[iterator].pulse_width);
      sendStimValue(t[iterator].channel_id, DAC_OP_MODE, (uint16_t) (STIM_ZERO - stim_value[iterator]));
      delayMicroseconds(t[iterator].pulse_width);
    }
    sendStimValue(t[iterator].channel_id, DAC_OP_MODE, (uint16_t) (STIM_ZERO));

    processing_time = micros() - processing_time;
    delayMicroseconds(t[0].t_total - processing_time);

    t_now = millis();
  }
}

uint16_t controlAlgorithm(control_type_t control_type, int channel, float control_p, float control_i, float control_d, float DT)
{
  float control_output;
  uint16_t control_factor;

  if (control_type == amplitude_modulated)
  {
    // Get error
    t[channel].error[1] = t[channel].error[0];
    t[channel].error[0] = t[channel].measured_data - t[channel].reference_data;

    // Calculate control variables
    control_p   =   t[channel].K_P * t[channel].error[0];
    control_i   =   control_i + t[channel].K_I * t[channel].error[0] * DT; // change to window
    control_d   =   t[channel].K_D * (t[channel].error[0] - t[channel].error[1]) / DT; // change error to window

    // Zero integrator
    if (t[channel].reference_data == 0)
      control_i = 0;

    // Change control output
    control_output = control_p + control_i + control_d;

    // Saturation and anti-windup
    // Control factor should not saturate
    // if (control_output > 1)
    // {
    //   control_output = 1;

    //   control_i = control_i - t[channel].K_I * (t[channel].error[0] + control_i) * DT; // anti-windup
    // }

    // Change control variable
    control_factor = control_output;
  }
  else
    control_factor = 1;

  return control_factor;
}

void initControlParameters()
{
  int iterator;

  for (iterator = 0; iterator < num_therapy_channels; iterator++)
  {
    t[iterator].K_P = 1;
    t[iterator].K_I = 0;
    t[iterator].K_D = 0;
  }
}

#define NORM_EXP(TIME_SCALE, AMP_SCALE) (exp(0.6931*dt/TIME_SCALE) - 1)*AMP_SCALE
// #define RC_SIN_FUNC_UP (max_amplitude * (sin(2*PI*1./(2*NEG_PW)*(dt))))
// #define RC_SIN_FUNC_DOWN (max_amplitude * (sin(2*PI*0.8/(2*NEG_PW)*(dt))))

uint16_t getCurrentAmplitudeValue(int dt, int polarity, float max_amplitude, float period)
{
  uint16_t stim = 0;

  if (waveform_type == rectangular)
    stim = (uint16_t)((STIM_MAX - STIM_ZERO) * max_amplitude);

  else if (waveform_type == exponential)
  {
    stim = (uint16_t)(NORM_EXP(period, (STIM_MAX - STIM_ZERO) * max_amplitude));
  }

  // else if (waveform_type == sinusoidal)
  // {
  //   if (dt < NEG_PW/2)
  //     stim = (uint16_t)RC_SIN_FUNC_UP;
  //   else
  //     stim = (uint16_t)RC_SIN_FUNC_DOWN;
  // }

  return (polarity * (stim + STIM_ZERO));
  // return (polarity*(stim));
}

void startRheobase()
{
  // Allocate variables according to number of channels
  d.measured_data = (float*) malloc(d.num_channels * sizeof(float));
  int iterator;

  d.amplitudes = (float*) malloc(d.num_channels * sizeof(float));
  for (iterator = 0; iterator < d.num_channels; iterator++)
    d.amplitudes[iterator] = 0;
  waveform_type = rectangular;

  ///zero la salida de los canales	
  //zeroChannels();

  // Get mean value for threshold
  // TODO: RMS calculation for data

  digitalWrite(RELAY_CH_1, LOW);//desligo la salida del canal 1 y la coloco en la precarga

  Serial.println("Starting rheobase callibration. Please stand still for 5 seconds.");

  d.accel_value = 0;
  d.accel_counter = 0;
  int t0 = millis();
  

  while (millis() - t0 < 4000)
  {
    d.accel_value += getFilteredSignal(ACCEL_PIN(0));
    d.accel_counter++;
  }

  d.accel_value /= d.accel_counter;

  digitalWrite(RELAY_CH_1, HIGH);//desligo la salida del canal 1 y la coloco en la precarg

  Serial.print("Callibration done! With samples: ");
  Serial.println(d.accel_counter);

  // Session loop
  while (!d.is_contracted)
  {
    // check botao de parada
    if (buttonStop())
    {
      d.is_contracted = true;
      zeroChannels();
      Serial.println("Operacao interrompida por botão.");
      finishCommunication();
      break;
    }
    // increase amplitude
    for (iterator = 0; iterator < d.num_channels; iterator++)
      d.amplitudes[iterator] += 0.01; // us

    // stimulate
    Serial.println("Amplitude: ");
    Serial.print(d.amplitudes[0] * 100);
    Serial.println(" mA");

    diagnosisStimPulse(d.amplitudes[0], RHEOBASE_PULSE_WIDTH); // TODO: change diagnosisStimPulse to multiple channels

    // TODO: check if operation was finished by other side

    // stream diagnosis response
    Serial.println("Streaming...");
    for (iterator = 0; iterator < d.num_channels; iterator++)
      d.measured_data[iterator] = d.amplitudes[iterator] * 100 * 1000; // * 100 para converter de ratio para mA. *1000 para converter de mA para uA
    streamDiagnosisMessage(true);
  }

  Serial.println("Contraction detected, finishing...");
  d.rheobase_amplitude = (float*)malloc(sizeof(float));
  *d.rheobase_amplitude = d.amplitudes[0];
  d.is_contracted = false;

  Serial.printf("Rehobase final amplitude: ");
  Serial.print(*d.rheobase_amplitude * 100);
  Serial.println(" mA");

  finishCommunication();
}

void startChronaxie()
{
  if (d.rheobase_amplitude == NULL)
  {
    // creating standard rheobase amplitude for debugging reasons
    d.rheobase_amplitude = (float*)malloc(sizeof(float));
    *d.rheobase_amplitude = 0.05; // valor de amplitude da cronaxia <- resultado da rheobase
    Serial.println("No rehobase amplitude");
    // TODO: finish operation (send error) if d.rheobase_amplitude == NULL
  }

  d.amplitudes[0] = *d.rheobase_amplitude;
  d.amplitudes[0] = 2 * d.amplitudes[0];
  Serial.print("Rheobase amplitude x 2: ");
  Serial.print(d.amplitudes[0] * 100); //Para mostrara el valor en mA
  Serial.println(" mA");

  d.measured_data = (float*) malloc(d.num_channels * sizeof(float));
  int iterator;

  d.pulse_widths = (float*) malloc(d.num_channels * sizeof(float));
  for (iterator = 0; iterator < d.num_channels; iterator++)
    d.pulse_widths[iterator] = 0;
  waveform_type = rectangular;

  ///zero la salida de los canales 
  //zeroChannels();
  digitalWrite(RELAY_CH_1, LOW);//desligo la salida del canal 1 y la coloco en la precarga

  // Get mean value for threshold
  Serial.println("Starting chronaxie callibration. Please stand still for 5 seconds.");
  d.accel_value = 0;
  d.accel_counter = 0;
  int t0 = millis();
  while (millis() - t0 < 4000)
  {
    d.accel_value += getFilteredSignal(ACCEL_PIN(0));
    d.accel_counter++;
  }

  d.accel_value /= d.accel_counter;

  digitalWrite(RELAY_CH_1, HIGH);//desligo la salida del canal 1 y la coloco en la precarg

  Serial.print("Callibration done! With samples: ");
  Serial.println(d.accel_counter);

  while (!d.is_contracted)
  {
    // check botao de parada
    if (buttonStop())
    {
      d.is_contracted = true;
      zeroChannels();
      Serial.println("Operacao interrompida por botão.");
      finishCommunication();
      break;
    }
    //Serial.println("Inside loop!");
    // increase pulse width
    for (iterator = 0; iterator < d.num_channels; iterator++)
      d.pulse_widths[iterator] += 50; // verificar en el osciloscopio la medida real, quitepara ir subiendo

    //d.pulse_widths[0] = 50;


    // stimulate
    Serial.println("Pulse width: ");
    Serial.print(d.pulse_widths[0]);//para mostra o valor em uS
    Serial.println(" uS");


    diagnosisStimPulse(d.amplitudes[0], d.pulse_widths[0]); // TODO: change diagnosisStimPulse to multiple channels

    // check if operation was finished by other side

    // stream diagnosis response
    Serial.println("Streaming...");
    for (iterator = 0; iterator < d.num_channels; iterator++)
      d.measured_data[iterator] = d.pulse_widths[iterator];
    streamDiagnosisMessage(true);
  }

  Serial.println("Contraction detected, finishing...");

  Serial.println("Chronaxie Final Pulse width: ");
  Serial.print(d.pulse_widths[0]); //para mostra o valor em uS
  Serial.println(" uS");

  d.is_contracted = false;

  finishCommunication();
}

void startAccommodation()
{
  d.measured_data = (float*) malloc(d.num_channels * sizeof(float));
  int iterator;

  d.amplitudes = (float*) malloc(d.num_channels * sizeof(float));
  for (iterator = 0; iterator < d.num_channels; iterator++)
    d.amplitudes[iterator] = 0;
  waveform_type = exponential;

  ///zero la salida de los canales	
  //zeroChannels();
  digitalWrite(RELAY_CH_1, LOW);//desligo la salida del canal 1 y la coloco en la precarga

  // stimulate
  d.amplitudes[0] = *d.rheobase_amplitude;//comienza en el valor de la reobase
  Serial.println("Start Acomodation Amplitude: ");
  Serial.print(d.amplitudes[0] * 100);
  Serial.println(" mA");


  // Get mean value for threshold
  Serial.println("Starting accommodation callibration. Please stand still for 5 seconds.");
  d.accel_value = 0;
  d.accel_counter = 0;
  int t0 = millis();
  while (millis() - t0 < 4000)
  {
    d.accel_value += getFilteredSignal(ACCEL_PIN(0));
    d.accel_counter++;
  }

  d.accel_value /= d.accel_counter;
  digitalWrite(RELAY_CH_1, HIGH);//desligo la salida del canal 1 y la coloco en la precarg

  Serial.print("Callibration done! With samples: ");
  Serial.println(d.accel_counter);

  while (!d.is_contracted)
  {
    // check botao de parada
    if (buttonStop())
    {
      d.is_contracted = true;
      zeroChannels();
      Serial.println("Operacao interrompida por botão.");
      finishCommunication();
      break;
    }
    // increase maximum amplitude
    for (iterator = 0; iterator < d.num_channels; iterator++)
      d.amplitudes[iterator] += 0.01;

    Serial.println("Amplitude: ");
    Serial.print(d.amplitudes[0] * 100);
    Serial.println(" mA");

    diagnosisStimPulse(d.amplitudes[0], RHEOBASE_PULSE_WIDTH); // TODO: change diagnosisStimPulse to multiple channels

    // check if operation was finished by other side

    // stream diagnosis response
    Serial.println("Streaming...");
    for (iterator = 0; iterator < d.num_channels; iterator++)
      d.measured_data[iterator] = d.amplitudes[iterator] * 100 * 1000;
    streamDiagnosisMessage(true);
  }

  Serial.println("Contraction detected, finishing...");

  Serial.printf("Acomodation final amplitude: ");
  Serial.print(d.amplitudes[0] * 100);
  Serial.println(" mA");

  d.is_contracted = false;

  finishCommunication();
}

void startOpenLoop()
{
  openLoopStimPulse();

  finishCommunication();
}

void startClosedLoop()
{
  closedLoopStimPulse(amplitude_modulated);

  finishCommunication();
}

void sendStimValue(int address, int operation_mode, uint16_t value)
{
  byte valueToWriteH = 0;
  byte valueToWriteL = 0;

  // Canais 2 e 4 estão invertidos no hardware
  if (address == 1)
    address = 3;
  else if (address == 3)
    address = 1;

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

void zeroChannels()
{
  digitalWrite(RELAY_CH_1, LOW);
  digitalWrite(RELAY_CH_2, LOW);
  digitalWrite(RELAY_CH_3, LOW);
  digitalWrite(RELAY_CH_4, LOW);
  for (int iterator = 0; iterator < 4; iterator++)
    sendStimValue(iterator, 1, (uint16_t)(STIM_ZERO + 20));
}

/* Accelerometer functions */
float getFilteredSignal(int pin)
{
  float sensor_value = analogRead(pin);

  //  filter it

  return sensor_value;
}

float getRMSAcceleration(int which_channel)
{
  float RMS = 0;
  circular_buffer[which_channel * BUFFER_SIZE + buffer_iterator[which_channel] % BUFFER_SIZE] = getFilteredSignal(ACCEL_PIN(which_channel));

  for (int iterator = 0; iterator < BUFFER_SIZE; iterator++)
    RMS += pow(circular_buffer[which_channel * BUFFER_SIZE + iterator], 2); // adding up all values in array
  RMS /= BUFFER_SIZE; // dividing by size of array

  return pow(RMS, .5);
}

boolean detectMovement(float accel, float mean)
{
  int threshold = 100;

  if (abs(accel - mean) > threshold)
  {
    Serial.println(String(abs(accel - mean)) + ">" + String(threshold));
    return true;
  }
  else
    return false;
}

void checkContraction()
{
  float accel;

  // get accel response
  accel = getFilteredSignal(ACCEL_PIN(0));

  // check if contraction
  if (detectMovement(accel, d.accel_value))
    d.is_contracted = true;
}

boolean changeParamsResponse(boolean verbose, byte* value, byte* update_message_id)
{
  byte* response;
  int response_length;
  int iterator, iterator_bytes;

  // Assemblying response
  response_length = 15;
  response = (byte*) malloc(response_length * sizeof(byte));
  response[0] = COMM_RESPONSE_START_BYTE;

  if (null_id)
    for (iterator_bytes = 0; iterator_bytes < 4; iterator_bytes++)
      response[1 + iterator_bytes] = 0;
  else {
    response[1] = update_message_id[0];
    response[2] = update_message_id[1];
    response[3] = update_message_id[2];
    response[4] = update_message_id[3];
  }

  response[5] = COMM_COMMAND_MESSAGE_THERAPY_UPDATE;
  response[6] = 0;
  response[7] = 5;
  response[8] = COMM_RESPONSE_RESULT_OK;
  response[9] = value[0];
  response[10] = value[1];
  response[11] = value[2];
  response[12] = value[3];

  // Checksum
  response[13] = 0;
  for (iterator = 1; iterator < 13; iterator++)
    response[13] += response[iterator];

  response[14] = COMM_END_BYTE;

  // Sending response
  sendBTData(response, response_length, verbose);

  return true;
}

boolean finishOperationResponse(boolean verbose)
{
  byte* response;
  int response_length;
  int iterator, iterator_bytes;

  // Assemblying response
  response_length = 15;
  response = (byte*) malloc(response_length * sizeof(byte));
  response[0] = COMM_RESPONSE_START_BYTE;
  if (null_id)
    for (iterator_bytes = 0; iterator_bytes < 4; iterator_bytes++)
      response[1 + iterator_bytes] = 0;
  else
    for (iterator_bytes = 0; iterator_bytes < 4; iterator_bytes++)
      response[1 + iterator_bytes] = message_id[iterator_bytes];
  response[5] = COMM_RESPONSE_FINISH_OPERATION;
  response[6] = 0;
  response[7] = 1;
  response[8] = COMM_RESPONSE_RESULT_OK;

  // Checksum
  response[9] = 0;
  for (iterator = 1; iterator < 9; iterator++)
    response[9] += response[iterator];

  response[10] = COMM_END_BYTE;

  // Sending response
  sendBTData(response, response_length, verbose);

  return true;
}

boolean buttonStop()
{
  int buttonState = 0;
  buttonState = digitalRead(STOP_BUTTON);
  if (buttonState == HIGH)
    return 1;
  else
    return 0;
}
