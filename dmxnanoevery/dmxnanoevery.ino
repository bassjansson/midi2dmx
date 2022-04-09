#include <DmxSimple.h>

#define DMX_TX_PIN 1
#define DMX_DE_PIN 2
#define DMX_NUM_CHAN 24

void setup() {
    // Initialize serial debug
    Serial.begin(115200);

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
    static String str = "";
    static int pos = 0;
    static uint8_t data[2] = {0, 0};

    while (Serial.available()) {
        char c = Serial.read();

        if (c == '\n') {
            if (pos < 2)
                data[pos] = str.toInt();

            DmxSimple.write(data[0], data[1]);

            str = "";
            pos = 0;
            data[0] = 0;
            data[1] = 0;
        }
        else if (c == ',') {
            if (pos < 2)
                data[pos] = str.toInt();

            str = "";
            pos++;
        }
        else {
            str += c;
        }
    }

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
