// inslude the SPI library:
#include <SPI.h>
// include DAC setup and flags
#include "DAC.h"

enum BraillePin {pinA, pinB, pinC, pinD, pinE, pinF};
// set pin 10 as the slave select for the digital pot:
const int dacSelect = 10;
const int buttonPin = 2;
int buttonState = 0;
void setup()
{
    // set the dacSelect as an output:
    pinMode (dacSelect, OUTPUT);
    // initialize SPI:
    SPI.begin();
    // buttonState
    pinMode(buttonPin, INPUT);
}
void loop()
{
    DAC_setWriteRegister(dacSelect);
    // Each channel values
    DAC_writeChannel(dacSelect, CHANNEL_H, VOLTAGE_5V);
    DAC_writeChannel(dacSelect, CHANNEL_G, VOLTAGE_5V);
    DAC_writeChannel(dacSelect, CHANNEL_F, VOLTAGE_5V);
    DAC_writeChannel(dacSelect, CHANNEL_E, VOLTAGE_5V);
    DAC_writeChannel(dacSelect, CHANNEL_D, VOLTAGE_5V);
    DAC_writeChannel(dacSelect, CHANNEL_C, VOLTAGE_5V);
    DAC_writeChannel(dacSelect, CHANNEL_B, VOLTAGE_5V);
    DAC_Broadcast(dacSelect, VOLTAGE_5V);

    buttonState = digitalRead(buttonPin);
    while(buttonState == LOW)
    {
        delay(30);
    }

}
// -- Operation Modes
// This operation mode writes on DACs register, no output change
void DAC_setWriteRegister(int dacPin)
{
    int16_t message = WRITE_REGISTER;
    DAC_write(dacPin, message);
}
// This operation mode changes directly the DACs output
void DAC_setWriteThrough(int dacPin)
{
    int16_t message = WRITE_OUTPUT;
    DAC_write(dacPin, message);
}
// -- DAC commands
// Updates values on SPI comunication
void DAC_write(int dacPin, int16_t message)
{
    // dacSelect = 0 means dac1:
    digitalWrite(dacSelect,LOW);
    // send in the address and value via SPI:
    SPI.transfer(message);
    // take the SS pin high to de-select the chip:
    digitalWrite(dacSelect,HIGH);
}
// Broadcast output to each register value, exept for pinA
void DAC_Broadcast(int dacPin, int voltageA)
{
    int valueA = DAC_getValue(voltageA);
    int message = DAC_setMessage(BROADCAST, valueA);
    DAC_write(dacPin, message);
}

int16_t DAC_getValue(int voltage)
{
    switch(voltage)
    {
        case 0:
            return VOLTAGE_0V;
            break;
        case 1:
            return VOLTAGE_1V;
            break;
        case 2:
            return VOLTAGE_2V;
            break;
        case 3:
            return VOLTAGE_3V;
            break;
        case 4:
            return VOLTAGE_4V;
            break;
        case 5:
            return VOLTAGE_5V;
            break;
        default:
            return VOLTAGE_0V;
            break;
    }
}

int16_t DAC_setMessage(int16_t opmode, int voltage)
{
    int value = DAC_getValue(voltage);
    return(opmode | value);
}

void DAC_writeChannel(int dacPin, int chNumber, int voltage)
{
    int16_t message;
    switch (chNumber)
    {
        case pinA:
            message = DAC_setMessage(CHANNEL_A, voltage);
            break;
        case pinB:
            message = DAC_setMessage(CHANNEL_B, voltage);
            break;
        case pinC:
            message = DAC_setMessage(CHANNEL_C, voltage);
            break;
        case pinD:
            message = DAC_setMessage(CHANNEL_D, voltage);
            break;
        case pinE:
            message = DAC_setMessage(CHANNEL_E, voltage);
            break;
        case pinF:
            message = DAC_setMessage(CHANNEL_F, voltage);
            break;
        default:
            return;
            break;
    }
    DAC_write(dacPin, message);
}
