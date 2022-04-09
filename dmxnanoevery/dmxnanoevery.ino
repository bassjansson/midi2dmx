#include <DmxSimple.h>

#define DMX_TX_PIN 1
#define DMX_DE_PIN 2
#define DMX_NUM_CHAN 24

void setup() {
    // Initialize serial debug
    // Serial.begin(115200);

    // Setup DMX
    DmxSimple.usePin(DMX_TX_PIN);
    DmxSimple.maxChannel(DMX_NUM_CHAN);

    // Enable DMX
    pinMode(DMX_DE_PIN, OUTPUT);
    digitalWrite(DMX_DE_PIN, HIGH);

    // Init lights
    DmxSimple.write(1, 255);
    DmxSimple.write(15, 255);
    DmxSimple.write(16, 255);
    DmxSimple.write(23, 255);
    DmxSimple.write(24, 255);
}

void loop() {
    unsigned long inter = 10ul * 1000ul; // ms

    // TODO: this modulo will be faulty once every 50 days
    double theta = fmod((double)(millis() % inter) / (double)inter + 0.25, 1.0);
    double wave = sin(theta * M_PI * 2.0) * 0.5 + 0.5;
    uint8_t value = (wave * wave * wave * wave) * 255;

    DmxSimple.write(4, value);
    DmxSimple.write(7, value);
    DmxSimple.write(9, value);
    DmxSimple.write(18, value);

    delay(20);
}

// // ATMEGA4809 on Arduino Nano Every
// #elif defined (__AVR_ATmega4809__)
// inline static void TIMER2_INTERRUPT_ENABLE() {
//   TCB0.INTFLAGS = TCB_CAPT_bm;
//   TCB0.INTCTRL  = TCB_CAPT_bm;
//   TCB0.CTRLA   |= TCB_ENABLE_bm;
//
//   TCB0.CTRLA |= TCB_CLKSEL_CLKTCA_gc; // 64 prescaler = 250 kHz
//   TCB0.CTRLB |= TCB_CNTMODE_INT_gc;   // compare count mode
//   TCB0.CCMP   = 65535ul;              // 16-bit max overflow
//   TCB0.CNT    = 0;                    // reset counter
// }
// inline static void TIMER2_INTERRUPT_DISABLE() {
//   TCB0.INTFLAGS =  TCB_CAPT_bm;
//   TCB0.INTCTRL &= ~TCB_CAPT_bm;
//   TCB0.CTRLA   &= ~TCB_ENABLE_bm;
// }
// #define ISR_NAME TCB0_INT_vect
// #define BITS_PER_TIMER_TICK (F_CPU / 31372)
