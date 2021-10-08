// This sketch sends a raw signal using the soft PWM sender every 5 seconds.
// It requires an IR-Led connected to the sending pin

#include <IrSenderPwmSpinWait.h>

static const frequency_t necFrequency = 38400U;
static const pin_t pin = 4U; // D2 on ESP8266
static const unsigned long BAUD = 115200UL;

// NEC(1) 122 29 with no repetition; powers on many Yamaha receivers
static const microseconds_t array[] = {
   9024, 4512, 564, 564, 564, 1692, 564, 564, 564, 1692, 564, 1692,
   564, 1692, 564, 1692, 564, 564, 564, 1692, 564, 564, 564, 1692,
   564, 564, 564, 564, 564, 564, 564, 564, 564, 1692, 564, 1692, 564,
   564, 564, 1692, 564, 1692, 564, 1692, 564, 564, 564, 564, 564, 564,
   564, 564, 564, 1692, 564, 564, 564, 564, 564, 564, 564, 1692, 564,
   1692, 564, 1692, 564, 39756
};

static const IrSequence irSequence(array, sizeof(array) / sizeof(microseconds_t));
IrSender* irSender;

void setup() {
    Serial.begin(BAUD);
    while (!Serial)
        ;
    irSender = new IrSenderPwmSpinWait(pin);
}

void loop() {
    Serial.print("sending on pin ");
    Serial.println(pin, DEC);
    irSender->send(irSequence, necFrequency, 33U);
    delay(5000);
}
