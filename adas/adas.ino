/*
  ---------------------------------------------------------------------------------
  ADS1292 for Arduino Code

  Inspired by:
  https://github.com/ericherman/eeg-mouse
  http://www.mccauslandcenter.sc.edu/CRNL/ads1298

  ---------------------------------------------------------------------------------
  -----------------------------------------------------------
  | Module | Signal         |   Teensy Pin                  |
  |--------+----------------+-------------------------------+
  |        |  CS            |  23                           |
  |  SPI   |  MOSI (DOUT)   |  7  (not library default)     |
  |        |  MISO (DIN)    |  8  (not library default)     |
  |        |  SCK           |  14 (not library default)     |
  |--------+----------------+-------------------------------+
  |        |  START         |  22                           |
  |        |  RESET         |  21                           |
  |  ADS   |  DRDY          |  4                            |
  |        |  CLKSEL        |                               |
  |        |  STOP          |                               |
  |        |  PWDN          |                               |
  -----------------------------------------------------------
  |        |  +5V           |  +5V                          |
  | Power  |  +3.3V         |  +3.3V                        |
  |        |  GND           |  GND                          |
  -----------------------------------------------------------


  SCLK   |  Device attempts to decode and execute commands every eight serial clocks.
       |  It is recommended that multiples of 8 SCLKs be presented every serial transfer to keep the interface in a normal operating mode.
       |
  DRDY   |  low = new data are available, regardless of CS
       |
  START  |  keep low if using start opcode. The START pin or the START command is used to place the device either in normal data capture mode or pulse data capture mode.
       |
  PWDN   |  When PWDN is pulled low, all on-chip circuitry is powered down
       |
  CS     |  low = SPI is active, must remain low during communication, then wait 4-5 tCLK cycles before returning to high, set CS high then low to reset the communication
       |  connect to J3.1 if JP21 = <**o], or J3.7 if JP21 = <o**]
       |
  RESET  |  low = force a reset, RESET is automatically issued to the digital filter whenever registers CONFIG1 and RESP are set to new values with a WREG command
       |
  CLKSEL |  internal clock: JP23 is <o**], external clock: JP23 is <**o]


  SPI library
  the spi clock divider (setClockDivider) sets relative to the system clock, the frequency of SLK relative to the chip frequency
  it needs to be at least 32, or the clock is too slow, 64 to be safe
  SPI.setClockDivider(SPI_CLOCK_DIV32);

  uint8_t = unsigned char with 8 bits

*/

//---------------------------------------------------------------------------------
// libraries

#include <SPI.h>

//---------------------------------------------------------------------------------
// vars

String sep = "---------------------------------------------------------";
boolean debug_msg     = true;
int n_channels      = 0;
int blank       = 0;

// SPI
#define DOUT 7 //MOSI
#define DIN 8 //MISO
#define SPICK 14 //SCK

// pins
#define PIN_CS  23
#define PIN_RESET  21
#define PIN_START  22
#define PIN_DRDY  4
#define PIN_LED  13

// register commands
#define READ  0x20
#define WRITE  0x40

// other
int gMaxChan = 2;
int gNumActiveChan = 2;
int activeSerialPort = 0; //data will be sent to serial port that last sent commands. E.G. bluetooth or USB port
boolean gActiveChan [2]; // reports whether channels 1..2 are active
boolean isRDATAC = true;

enum spi_command {
  // system commands
  WAKEUP = 0x02,
  STANDBY = 0x04,
  RESET = 0x06,
  START = 0x08,
  STOP = 0x0a,

  // read commands
  RDATAC = 0x10,
  SDATAC = 0x11,
  RDATA = 0x12,

  // register commands
  RREG = 0x20,
  WREG = 0x40
};

enum reg {
  // device settings
  ID = 0x00,

  // global settings
  CONFIG1 = 0x01,
  CONFIG2 = 0x02,
  LOFF = 0x03,

  // channel specific settings
  CHnSET = 0X03,
  CH1SET = CHnSET + 1,
  CH2SET = CHnSET + 2,

  // lead off status
  RLD_SENS = 0x06,
  LOFF_SENS = 0x07,
  LOFF_STAT = 0x08,

  // other
  GPIO = 0x0B,
  RESP1 = 0x09,
  RESP2 = 0x0A

};

enum ID_bits {
  DEV_ID7 = 0x80,
  DEV_ID6 = 0x40,
  DEV_ID5 = 0x20,
  DEV_ID2 = 0x04,
  DEV_ID1 = 0x02,
  DEV_ID0 = 0x01,

  ID_const = 0x10,
  ID_ADS129x = DEV_ID7,
  ID_ADS129xR = (DEV_ID7 | DEV_ID6),

  ID_4CHAN = 0,
  ID_6CHAN = DEV_ID0,
  ID_8CHAN = DEV_ID1,

  ID_ADS1294 = (ID_ADS129x | ID_4CHAN),
  ID_ADS1296 = (ID_ADS129x | ID_6CHAN),
  ID_ADS1298 = (ID_ADS129x | ID_8CHAN),
  ID_ADS1294R = (ID_ADS129xR | ID_4CHAN),
  ID_ADS1296R = (ID_ADS129xR | ID_6CHAN),
  ID_ADS1298R = (ID_ADS129xR | ID_8CHAN)
};

enum CONFIG1_bits {
  SINGLE_SHOT = 0x80,
  DR2 = 0x04,
  DR1 = 0x02,
  DR0 = 0x01,

  CONFIG1_const = 0x00,
  SAMP_125_SPS = CONFIG1_const,
  SAMP_250_SPS = (CONFIG1_const | DR0),
  SAMP_500_SPS = (CONFIG1_const | DR1),
  SAMP_1_KSPS = (CONFIG1_const | DR1 | DR0),
  SAMP_2_KSPS = (CONFIG1_const | DR2),
  SAMP_4_KSPS = (CONFIG1_const | DR2 | DR0),
  SAMP_8_KSPS = (CONFIG1_const | DR2 | DR1)

};

enum CONFIG2_bits {
  PDB_LOFF_COMP = 0x40,
  PDB_REFBUF = 0X20,
  VREF_4V = 0x10,
  CLK_EN = 0X08,
  INT_TEST = 0x02, //amplitude = Â±(VREFP â€“ VREFN) / 2400
  TEST_FREQ = 0x01,

  CONFIG2_const = 0x80,
  INT_TEST_1HZ = (CONFIG2_const | INT_TEST | TEST_FREQ),
  INT_TEST_DC = (CONFIG2_const | INT_TEST)
};

enum LOFF_bits {
  COMP_TH2 = 0x80,
  COMP_TH1 = 0x40,
  COMP_TH0 = 0x20,
  VLEAD_OFF_EN = 0x10,
  ILEAD_OFF1 = 0x08,
  ILEAD_OFF0 = 0x04,
  FLEAD_OFF1 = 0x02,
  FLEAD_OFF0 = 0x01,

  LOFF_const = 0x00,

  COMP_TH_95 = 0x00,
  COMP_TH_92_5 = COMP_TH0,
  COMP_TH_90 = COMP_TH1,
  COMP_TH_87_5 = (COMP_TH1 | COMP_TH0),
  COMP_TH_85 = COMP_TH2,
  COMP_TH_80 = (COMP_TH2 | COMP_TH0),
  COMP_TH_75 = (COMP_TH2 | COMP_TH1),
  COMP_TH_70 = (COMP_TH2 | COMP_TH1 | COMP_TH0),

  ILEAD_OFF_6nA = 0x00,
  ILEAD_OFF_12nA = ILEAD_OFF0,
  ILEAD_OFF_18nA = ILEAD_OFF1,
  ILEAD_OFF_24nA = (ILEAD_OFF1 | ILEAD_OFF0),

  FLEAD_OFF_AC = FLEAD_OFF0,
  FLEAD_OFF_DC = (FLEAD_OFF1 | FLEAD_OFF0)
};

enum CHnSET_bits {
  PDn = 0x80,
  PD_n = 0x80,
  GAINn2 = 0x40,
  GAINn1 = 0x20,
  GAINn0 = 0x10,
  MUXn2 = 0x04,
  MUXn1 = 0x02,
  MUXn0 = 0x01,

  CHnSET_const = 0x00,

  GAIN_1X = GAINn0,
  GAIN_2X = GAINn1,
  GAIN_3X = (GAINn1 | GAINn0),
  GAIN_4X = GAINn2,
  GAIN_6X = 0x00,
  GAIN_8X = (GAINn2 | GAINn0),
  GAIN_12X = (GAINn2 | GAINn1),

  ELECTRODE_INPUT = 0x00,
  SHORTED = MUXn0,
  RLD_INPUT = MUXn1,
  MVDD = (MUXn1 | MUXn0),
  TEMP = MUXn2,
  TEST_SIGNAL = (MUXn2 | MUXn0),
  RLD_DRP = (MUXn2 | MUXn1),
  RLD_DRN = (MUXn2 | MUXn1 | MUXn0)
};

enum CH1SET_bits {
  PD_1 = 0x80,
  GAIN12 = 0x40,
  GAIN11 = 0x20,
  GAIN10 = 0x10,
  MUX12 = 0x04,
  MUX11 = 0x02,
  MUX10 = 0x01,

  CH1SET_const = 0x00
};

enum CH2SET_bits {
  PD_2 = 0x80,
  GAIN22 = 0x40,
  GAIN21 = 0x20,
  GAIN20 = 0x10,
  MUX22 = 0x04,
  MUX21 = 0x02,
  MUX20 = 0x01,

  CH2SET_const = 0x00
};

enum RLD_SENSP_bits {
  RLD8P = 0x80,
  RLD7P = 0x40,
  RLD6P = 0x20,
  RLD5P = 0x10,
  RLD4P = 0x08,
  RLD3P = 0x04,
  RLD2P = 0x02,
  RLD1P = 0x01,

  RLD_SENSP_const = 0x00
};

enum RLD_SENSN_bits {
  RLD8N = 0x80,
  RLD7N = 0x40,
  RLD6N = 0x20,
  RLD5N = 0x10,
  RLD4N = 0x08,
  RLD3N = 0x04,
  RLD2N = 0x02,
  RLD1N = 0x01,

  RLD_SENSN_const = 0x00
};

enum LOFF_SENSP_bits {
  LOFF8P = 0x80,
  LOFF7P = 0x40,
  LOFF6P = 0x20,
  LOFF5P = 0x10,
  LOFF4P = 0x08,
  LOFF3P = 0x04,
  LOFF2P = 0x02,
  LOFF1P = 0x01,

  LOFF_SENSP_const = 0x00
};

enum LOFF_SENSN_bits {
  LOFF8N = 0x80,
  LOFF7N = 0x40,
  LOFF6N = 0x20,
  LOFF5N = 0x10,
  LOFF4N = 0x08,
  LOFF3N = 0x04,
  LOFF2N = 0x02,
  LOFF1N = 0x01,

  LOFF_SENSN_const = 0x00
};

enum LOFF_FLIP_bits {
  LOFF_FLIP8 = 0x80,
  LOFF_FLIP7 = 0x40,
  LOFF_FLIP6 = 0x20,
  LOFF_FLIP5 = 0x10,
  LOFF_FLIP4 = 0x08,
  LOFF_FLIP3 = 0x04,
  LOFF_FLIP2 = 0x02,
  LOFF_FLIP1 = 0x01,

  LOFF_FLIP_const = 0x00
};

enum LOFF_STATP_bits {
  IN8P_OFF = 0x80,
  IN7P_OFF = 0x40,
  IN6P_OFF = 0x20,
  IN5P_OFF = 0x10,
  IN4P_OFF = 0x08,
  IN3P_OFF = 0x04,
  IN2P_OFF = 0x02,
  IN1P_OFF = 0x01,

  LOFF_STATP_const = 0x00
};

enum LOFF_STATN_bits {
  IN8N_OFF = 0x80,
  IN7N_OFF = 0x40,
  IN6N_OFF = 0x20,
  IN5N_OFF = 0x10,
  IN4N_OFF = 0x08,
  IN3N_OFF = 0x04,
  IN2N_OFF = 0x02,
  IN1N_OFF = 0x01,

  LOFF_STATN_const = 0x00
};

enum GPIO_bits {
  GPIOC2 = 0x08,
  GPIOC1 = 0x04,

  GPIOD2 = 0x02,
  GPIOD1 = 0x01,

  GPIO_const = 0x00
};

enum RESP_bits {
  RESP_DEMOD_EN1 = 0x80,
  RESP_MOD_EN1 = 0x40,
  RESP_PH2 = 0x10,
  RESP_PH1 = 0x08,
  RESP_PH0 = 0x04,
  RESP_CTRL1 = 0x02,
  RESP_CTRL0 = 0x01,

  RESP_const = 0x20,

  RESP_PH_22_5 = 0x00,
  RESP_PH_45 = RESP_PH0,
  RESP_PH_67_5 = RESP_PH1,
  RESP_PH_90 = (RESP_PH1 | RESP_PH0),
  RESP_PH_112_5 = RESP_PH2,
  RESP_PH_135 = (RESP_PH2 | RESP_PH0),
  RESP_PH_157_5 = (RESP_PH2 | RESP_PH1),

  RESP_NONE = 0x00,
  RESP_EXT = RESP_CTRL0,
  RESP_INT_SIG_INT = RESP_CTRL1,
  RESP_INT_SIG_EXT = (RESP_CTRL1 | RESP_CTRL0)
};

// serial api
boolean read_ads_data     = false;
boolean serial_send_data  = false;
boolean valid_cmd               = false;
enum SERIAL_API {
  S_READ_ADS = 0x67,//      = 0xBF,   // read from the ads
  S_STOP_READ_ADS = 0x73, //  = 0xFD,   // stop reading freom the ads
  S_SEND_SERIAL     = 0xDF,   // start sending to the serial interface
  S_STOP_SEND_SERIAL  = 0xFB    // stop streaming to the serial interface
};


//---------------------------------------------------------------------------------
// functions

//utilities
#include <stdarg.h>

String hex_to_char(int hex_in) {
  int precision = 2;
  char tmp[16];
  char format[128];
  sprintf(format, "0x%%.%dX", precision);
  sprintf(tmp, format, hex_in);
  //Serial.print(tmp);
  return (String(tmp));
}

void write_byte(int reg_addr, int val_hex) {
  //see pages 40,43 of datasheet -
  digitalWrite(PIN_CS, LOW);
  delayMicroseconds(5);
  SPI.transfer(0x40 | reg_addr);
  delayMicroseconds(5);
  SPI.transfer(0x00); // number of registers to be read/written â€“ 1
  delayMicroseconds(5);
  SPI.transfer(val_hex);
  delayMicroseconds(10);
  digitalWrite(PIN_CS, HIGH);

  if (debug_msg) {
    Serial.println(sep);
    Serial.print( "sent:\t" + hex_to_char(reg_addr) + "\t" + hex_to_char(val_hex) + "\t" );
    Serial.println(val_hex, BIN);
  }
}

int read_byte(int reg_addr) {
  int out = 0;
  digitalWrite(PIN_CS, LOW);
  SPI.transfer(0x20 | reg_addr);
  delayMicroseconds(5);
  SPI.transfer(0x00); // number of registers to be read/written â€“ 1
  delayMicroseconds(5);
  out = SPI.transfer(0x00);
  delayMicroseconds(1);
  digitalWrite(PIN_CS, HIGH);

  if (debug_msg) {
    Serial.println(sep);
    Serial.println( "sent:\t" + hex_to_char(reg_addr) );
    Serial.println( "recieved:\t" + hex_to_char(out) );
  }

  return (out);
}

void send_command(uint8_t cmd) {
  digitalWrite(PIN_CS, LOW);
  delayMicroseconds(5); // 5 = 6us at 32
  SPI.transfer(cmd);
  delayMicroseconds(10);
  digitalWrite(PIN_CS, HIGH);
}

// initialization

void init_pins() {
  pinMode(SPICK, OUTPUT);
  pinMode(DIN, INPUT);
  pinMode(DOUT, OUTPUT);
  pinMode(PIN_LED,   OUTPUT);
  pinMode(PIN_CS,    OUTPUT);
  pinMode(PIN_RESET, OUTPUT);
  pinMode(PIN_START, OUTPUT);
  pinMode(PIN_DRDY, INPUT);
  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_START, LOW);
  delay(1);
}

void init_serial() {
  Serial.begin(115200);
  Serial.flush();
  delayMicroseconds(100);
}

void init_spi() {

  SPI.setMOSI(DOUT);
  SPI.setMISO(DIN);
  SPI.setSCK(SPICK);

  // initializes the SPI bus by setting SCK and MOSI low
  SPI.begin();

  // spi data mode
  // sets clock polarity and phase
  // CPOL = 0 (clock polarity, clock is idle when low)
  // CPHA = 1 (clock phase , data is shifted in and out on the rising of the data clock signal )
  SPI.setDataMode(SPI_MODE1);

  // spi clock divider
  // sets relative to the system clock
  // n transitions per cycles (SPI_CLOCK_DIV2 = 1 transition / 2 cycles)
  // DIV4 is arduino default, override to make faster
  // needs to be at least 32, or the clock is too slow, 64 to be safe
  SPI.setClockDivider(SPI_CLOCK_DIV16);

  // spi bit order
  // sets the order of the bits shifted in and out
  // MSBFIRST = most-significant bit first
  SPI.setBitOrder(MSBFIRST);

  // Pause
  delay(1);
}

void init_ads() {
  int chSet;
  // see page 77 for boot sequence

  // Issue Reset Pulse
  digitalWrite(PIN_RESET, HIGH);
  delay(1000);
  digitalWrite(PIN_RESET, LOW);
  delay(1000);
  digitalWrite(PIN_RESET, HIGH);
  delay(100);

  // Reset communication
  digitalWrite(PIN_CS, LOW);
  delay(1000);
  digitalWrite(PIN_CS, HIGH);

  // Wait longer for TI chip to start
  delay(500);

  // Send SDATAC Command (Stop Read Data Continuously mode)
  send_command(SDATAC);
  delay(10);

  chSet = read_byte(READ | ID);
  Serial.print("-- ID" + String(chSet) + "--");

  // All GPIO set to output 0x0000: (floating CMOS inputs can flicker on and off, creating noise)
  write_byte(GPIO, 0x00);

  if (debug_msg) {
    Serial.println(sep);
    Serial.println("CONFIGs 1 2");
  }
  write_byte(CONFIG1, SAMP_500_SPS);
  write_byte(CONFIG2, 0xA0);
  delay(1000);
  write_byte(CONFIG2, 0xA3); //Actives test signal. Comment this line for normal electrodes

  if (debug_msg) {
    Serial.println(sep);
    Serial.println("Check Configs");
    chSet = read_byte(CONFIG1);
    Serial.println("CONFIG1: " + String(chSet) + "\t\t" + hex_to_char(chSet) );
    chSet = read_byte(CONFIG2);
    Serial.println("CONFIG2: " + String(chSet) + "\t\t" + hex_to_char(chSet) );
  }

  if (debug_msg) {
    Serial.println(sep);
    Serial.println("Set Channels");
  }
  // Set channels to input signal
  for (int i = 1; i <= gMaxChan; ++i) {
    //write_byte(CHnSET + i, ELECTRODE_INPUT | GAIN_12X);
    write_byte(CHnSET + i, 0x05); //For test signal 0x05. For normal electrodes 0x00
    //write_byte(CHnSET + i,SHORTED);
  }

  // Start
  digitalWrite(PIN_START, HIGH);
  delay(150);

  // get device id

  //detect active channels
  if (debug_msg) {
    Serial.println(sep);
    Serial.println("detecting active channels:");
  }
  gNumActiveChan = 0;

  for (int i = 1; i <= gMaxChan; i++) {
    delayMicroseconds(1);
    chSet = read_byte(CHnSET + i);
    gActiveChan[i] = ((chSet & 7) != SHORTED);          // SHORTED = 0x01
    if ( (chSet & 7) != SHORTED) gNumActiveChan++;
    if (debug_msg) Serial.println(String(i) + ": " + String(chSet) + "\t\t" + hex_to_char(chSet) );
  }
  if (debug_msg) Serial.println("detected " + String(gNumActiveChan) + " active channels.");

  // start reading
  //send_command(RDATAC); // can't read registers when in rdatac mode!

}

// get data

void read_and_send_data() {

  //Serial.println(sep);

  //vars
  int numSerialBytes = 1 + (3 * gNumActiveChan); //8-bits header plus 24-bits per ACTIVE channel
  unsigned char serialBytes[numSerialBytes];
  int i = 0;

  int values[gNumActiveChan];

  unsigned int a, b, c;

  // start
  digitalWrite(PIN_CS, LOW);
  delayMicroseconds(1);
  //Serial.println("RD");
  SPI.transfer(RDATA);
  delayMicroseconds(1);
  // get bytes 1-3
  serialBytes[i++] = SPI.transfer(0x00); // get 1st byte of header
  //delayMicroseconds(1);
  SPI.transfer(0x00); //skip 2nd byte of header
  //delayMicroseconds(1);
  SPI.transfer(0x00); //skip 3rd byte of header
  delayMicroseconds(1);

  // get channels
  for (int ch = 1; ch <= gMaxChan; ch++) {

    a = SPI.transfer(0x00);
    //delayMicroseconds(1);
    b = SPI.transfer(0x00);
    //delayMicroseconds(1);
    c = SPI.transfer(0x00);
    //delayMicroseconds(1);

    if (gActiveChan[ch]) {
      // save 3 bytes per active channel
      serialBytes[i++] = a;
      serialBytes[i++] = b;
      serialBytes[i++] = c;
    }


    if (serial_send_data) { //Here we send the data. Now it only send channel 1 (ch == 1)
      //if(ch < gMaxChan){
      if (ch == 1) {
        //Serial.println("-Channel " + String(ch));
        Serial.print( String(a) + " " + String(b) + " " + String(c) + "\n");

      }
    }
  }

  // end
  delayMicroseconds(1);
  //delay(1);
  digitalWrite(PIN_CS, HIGH);
}

void serial_api_get() {

  // send data only when you receive data:
  if (Serial.available() > 0) {

    int serial_in = Serial.read();      // read the incoming byte:

    // process api
    if (serial_in == S_READ_ADS )    {
      read_ads_data = true;
      valid_cmd = true;
    }
    if (serial_in == S_STOP_READ_ADS ) {
      read_ads_data = false;
      valid_cmd = true;
    }
    if (serial_in == S_SEND_SERIAL )   {
      serial_send_data = true;
      digitalWrite(PIN_LED, HIGH);
      valid_cmd = true;
    }
    if (serial_in == S_STOP_SEND_SERIAL )    {
      serial_send_data = false;
      digitalWrite(PIN_LED, LOW);
      valid_cmd = true;
    }
    if (valid_cmd == false)                 {
      Serial.println("Invalid Command --> ");
      Serial.println(serial_in);
      Serial.println(" <--");
    }
    valid_cmd == false;
  }
}

void serial_api_use() {

  if (read_ads_data) {
    if (digitalRead(PIN_DRDY) == LOW) {
      read_and_send_data();       //only read when data is ready, sending data across serial is in this function!
    }
  }
}

// main

void setup() {

  // ------------------------------------------------------------
  // init
  delay(3000);

  // set pins
  init_pins();

  // serial
  init_serial();

  // on!
  if (debug_msg) Serial.println("on");
  digitalWrite(PIN_LED, HIGH);

  // spi
  init_spi();

  // ads
  init_ads();

  // ------------------------------------------------------------
  // do stuff

  // ------------------------------------------------------------
  // exit

  // off!
  digitalWrite(PIN_LED, LOW);
  if (debug_msg) Serial.println("off");
  //blinkyblink();

}

void loop() {
  serial_api_get();
  serial_api_use();
}










/*

  Joao

  // Data packets & commands from phone
  const int END                   = 'f';                     //Last character of packets (or 0xFF)

  //ecg data packet bytes: 8 + 8 + 8 + 16 + ~ + 8 bits
  const int STARTecg              = 'e';                     //First character of ecg packets
  uint8_t MODE                    = 0x00;                    //default is 12-lead
  /*ECG mode:
  00 â†’ 12-lead;
  01 â†’ 5-lead;
  10 â†’ 3-lead;
  11 â†’ 1-lead;
  2-7: Unused (set to 0)

  uint8_t ecgSFREQ                = 0x02;                    //default is 500 SPS
  // Sampling frequency, ecgSFREQ:
  // 250 SPS = 0x01;     4 KSPS = 0x05;
  // 500 SPS = 0x02;     8 KSPS = 0x06;
  // 1 KSPS = 0x03;      16 KSPS = 0x07;
  // 2 KSPS = 0x04;      32 KSPS = 0x08;
  // unsigned int data_struct        = 0x000;                   //Structure of data inside ecgData

  // data_struct bits / sensor status:
  // 0: ECG_V1;    6: ECG_LA;
  // 1: ECG_V2;    7: ECG_RA;
  // 2: ECG_V3;    8: ECG_LL;
  // 3: ECG_V4;    9: ECG_RL;
  // 4: ECG_V5;    13-16: Unused (Set to 0)
  // 5: ECG_V6;

  //ecg data: Maximum of 12-leads at 24-bits per samples = 12*3 bytes
  uint8_t ecgData[36]             = {0};
  const int ecgDataSize           = sizeof(ecgData);         // ecgData = 36

  //accelerometer data packet bytes: 8 + 8 + 16 + 16 + 16 + 8 bits
  const int  STARTacc             = 'a';                     //First character of accelerometer packets
  uint8_t accSFREQ                = 0x07;                    //6.5 SPS = 0x01; 12.5 SPS = 0x02; 25 SPS = 0x03; 50 SPS = 0x04; 100 SPS = 0x05; 200 SPS = 0x06; 400 SPS = 0x07; 800 SPS = 0x08; 1600 SPS = 0x09; 3200 SPS = 0x10
  //uint8_t accX[2]               = 0;
  unsigned int accX               = 0x000;
  //uint8_t accY[2]               = 0;
  unsigned int accY               = 0x000;
  //uint8_t accZ[2]               = 0;
  unsigned int accZ               = 0x000;

  //command data packet bytes: 8 + 8 + 8 + 8 bits
  uint8_t STARTcmd                = 'c';                     // start command  or 0x01;   //First character of command packets
  uint8_t REG                     = 'e';                     // default REG is RESET = 0x06
  uint8_t REGval                  = 0x06;
  /*REG definitions from phone
  REG = 'u'; REGval = 0x02; â†’ WAKEUP
  REG = 'b'; REGval = 0x04; â†’ STANDBY
  REG = 'e'; REGval = 0x06; â†’ RESET
  REG = 't'; REGval = 0x08; â†’ START
  REG = 'p'; REGval = 0x0a; â†’ STOP
  REG = 'c'; REGval = 0x10; â†’ RDATAC
  REG = 's'; REGval = 0x11; â†’ SDATAC
  REG = 'r'; REGval = 0x12; â†’ RDATA
  REG = 'g'; REGval = 0x20; â†’ READ_REG
  REG = 'w'; REGval = 0x40; â†’ WRITE_REG

*/
