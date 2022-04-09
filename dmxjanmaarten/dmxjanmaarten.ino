#include <DmxSimple.h>

#define DMX_TX_PIN 1
#define DMX_DE_PIN 2
#define DMX_NUM_CHAN 6

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
    DmxSimple.write(1, 255); // Master dimmer
    DmxSimple.write(2, 0); // Strobe / Speed

    DmxSimple.write(3, 255); // Master dimmer
    DmxSimple.write(4, 0); // Strobe / Speed

    DmxSimple.write(5, 255); // Master dimmer
    DmxSimple.write(6, 0); // Strobe / Speed
}

void loop() {
    unsigned long inter = 6ul * 60ul * 1000ul; // ms

    // TODO: this modulo will be faulty once every 50 days
    double theta = fmod((double)(millis() % inter) / (double)inter + 0.25, 1.0);
    double wave = sin(theta * M_PI * 2.0) * 0.5 + 0.5;
    uint8_t value = (wave * wave * wave * wave) * 128;

    DmxSimple.write(1, value); // Segment 1
    DmxSimple.write(3, value); // Segment 2
    DmxSimple.write(5, value); // Segment 3

    delay(20);
}
