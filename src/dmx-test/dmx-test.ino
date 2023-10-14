#define ENABLE_SERIAL_DEBUG // Comment this define to disable serial debugging

#include <DmxSimple.h>

#define DMX_TX_PIN   16 // Transmit pin, connect to DI of MAX485 module
#define DMX_DE_PIN   9  // Enable pin, connect to DE+RE of MAX485 module

#define DMX_NUM_CHAN 24 // Number of DMX channels to use, maximum is 512 (if sufficient RAM)

void setup()
{
#ifdef ENABLE_SERIAL_DEBUG
    // Initialize serial communication
    Serial.begin(115200);
    // while (!Serial) // Wait for native USB
    //     delay(1);
#endif

    // Setup DMX
    DmxSimple.usePin(DMX_TX_PIN);
    DmxSimple.maxChannel(DMX_NUM_CHAN);

    // Enable DMX
    pinMode(DMX_DE_PIN, OUTPUT);
    digitalWrite(DMX_DE_PIN, HIGH);

    // Init all DMX channels
    initDmxChannels();
} // setup

void loop()
{
    // Color test: show each basic color for 2 seconds

    // Turn on RED channels only for 2 seconds
    writeRgbToDmx(255, 0, 0);
#ifdef ENABLE_SERIAL_DEBUG
    Serial.println("RED");
#endif
    delay(2000);

    // Turn on GREEN channels only for 2 seconds
    writeRgbToDmx(0, 255, 0);
#ifdef ENABLE_SERIAL_DEBUG
    Serial.println("GREEN");
#endif
    delay(2000);

    // Turn on BLUE channels only for 2 seconds
    writeRgbToDmx(0, 0, 255);
#ifdef ENABLE_SERIAL_DEBUG
    Serial.println("BLUE");
#endif
    delay(2000);

    // Set lights to MAGENTA for 2 seconds
    writeRgbToDmx(255, 0, 255);
#ifdef ENABLE_SERIAL_DEBUG
    Serial.println("MAGENTA");
#endif
    delay(2000);

    // Set lights to YELLOW for 2 seconds
    writeRgbToDmx(255, 255, 0);
#ifdef ENABLE_SERIAL_DEBUG
    Serial.println("YELLOW");
#endif
    delay(2000);

    // Set lights to CYAN for 2 seconds
    writeRgbToDmx(0, 255, 255);
#ifdef ENABLE_SERIAL_DEBUG
    Serial.println("CYAN");
#endif
    delay(2000);
}

void initDmxChannels()
{
    // Init your lights here.
    // It might be that some of your lights need some channels
    // to be on another state than 0 before they turn on at all.
    // For example, I have some lights which need some channels
    // to be 255 to turn on.

    // ==> UPDATE THE NUMBERS BELOW WITH YOUR DMX CHANNEL CONFIGURATION! <==

    DmxSimple.write(1, 255);
    DmxSimple.write(2, 0); // R
    DmxSimple.write(3, 0); // G
    DmxSimple.write(4, 0); // B
    DmxSimple.write(5, 0);
    DmxSimple.write(6, 0);

    DmxSimple.write(7, 0); // W
    DmxSimple.write(8, 0);

    DmxSimple.write(9, 0);  // R
    DmxSimple.write(10, 0); // G
    DmxSimple.write(11, 0); // B
    DmxSimple.write(12, 0); // W
    DmxSimple.write(13, 0); // A
    DmxSimple.write(14, 0); // UV
    DmxSimple.write(15, 255);
    DmxSimple.write(16, 255);

    DmxSimple.write(17, 0); // R
    DmxSimple.write(18, 0); // G
    DmxSimple.write(19, 0); // B
    DmxSimple.write(20, 0); // W
    DmxSimple.write(21, 0); // A
    DmxSimple.write(22, 0); // UV
    DmxSimple.write(23, 255);
    DmxSimple.write(24, 255);
}

void writeRgbToDmx(uint8_t r, uint8_t g, uint8_t b)
{
    // ==> UPDATE THE NUMBERS BELOW WITH YOUR DMX CHANNEL CONFIGURATION! <==

    DmxSimple.write(2, r); // R
    DmxSimple.write(3, g); // G
    DmxSimple.write(4, b); // B

    DmxSimple.write(9, r);  // R
    DmxSimple.write(10, g); // G
    DmxSimple.write(11, b); // B

    DmxSimple.write(17, r); // R
    DmxSimple.write(18, g); // G
    DmxSimple.write(19, b); // B
}
