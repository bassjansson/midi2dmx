// #define ENABLE_SERIAL_DEBUG // Uncomment this define to enable serial debugging
// #define USING_USB_MIDI // Uncomment this define to use USB-MIDI

#ifndef USING_USB_MIDI
    // DIN-5 Serial MIDI
    #include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();
#else
    // USB-MIDI
    #include <USB-MIDI.h>
USBMIDI_CREATE_DEFAULT_INSTANCE();
using namespace MIDI_NAMESPACE;
#endif

#include <DmxSimple.h>

#define ON_LED_PIN   13 // Built-in LED, change if using another pin

#define DMX_TX_PIN   4 // Transmit pin, connect to DI of MAX485 module
#define DMX_DE_PIN   2 // Enable pin, connect to DE+RE of MAX485 module

#define MIDI_CHANNEL MIDI_CHANNEL_OMNI // MIDI channel to listen to, omni means all channels
#define DMX_NUM_CHAN 24                // Number of DMX channels to use, maximum is 512 (if sufficient RAM)

void setup()
{
#ifdef ENABLE_SERIAL_DEBUG
    // Initialize serial communication
    Serial.begin(115200);
    while (!Serial) // Wait for native USB
        delay(1);
#endif

    // Setup and turn on the 'on' LED
    pinMode(ON_LED_PIN, OUTPUT);
    digitalWrite(ON_LED_PIN, HIGH);

    // Listen for MIDI messages on specified channel
    MIDI.begin(MIDI_CHANNEL);

    // Setup all MIDI event callbacks
    MIDI.setHandleNoteOn(OnNoteOn);
    MIDI.setHandleNoteOff(OnNoteOff);
    MIDI.setHandleAfterTouchPoly(OnAfterTouchPoly);
    MIDI.setHandleControlChange(OnControlChange);
    MIDI.setHandleProgramChange(OnProgramChange);
    MIDI.setHandleAfterTouchChannel(OnAfterTouchChannel);
    MIDI.setHandlePitchBend(OnPitchBend);
    MIDI.setHandleSystemExclusive(OnSystemExclusive);
    MIDI.setHandleTimeCodeQuarterFrame(OnTimeCodeQuarterFrame);
    MIDI.setHandleSongPosition(OnSongPosition);
    MIDI.setHandleSongSelect(OnSongSelect);
    MIDI.setHandleTuneRequest(OnTuneRequest);
    MIDI.setHandleClock(OnClock);
    MIDI.setHandleStart(OnStart);
    MIDI.setHandleContinue(OnContinue);
    MIDI.setHandleStop(OnStop);
    MIDI.setHandleActiveSensing(OnActiveSensing);
    MIDI.setHandleSystemReset(OnSystemReset);

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
    // Listen to incoming notes
    MIDI.read();
}

void initDmxChannels()
{
    // Init your lights here.
    // It might be that some of your lights need some channels
    // to be on another state than 0 before they go on at all.
    // For example, I have some lights which need some channels
    // to be 255 to go on.
    DmxSimple.write(1, 0);
    DmxSimple.write(2, 0);
    DmxSimple.write(3, 0);
    DmxSimple.write(4, 0);
    DmxSimple.write(5, 0);
    DmxSimple.write(6, 0);
    DmxSimple.write(7, 0);
    DmxSimple.write(8, 0);
    DmxSimple.write(9, 0);
    DmxSimple.write(10, 0);
    DmxSimple.write(11, 0);
    DmxSimple.write(12, 0);
    DmxSimple.write(13, 0);
    DmxSimple.write(14, 0);
    DmxSimple.write(15, 0);
    DmxSimple.write(16, 0);
    DmxSimple.write(17, 0);
    DmxSimple.write(18, 0);
    DmxSimple.write(19, 0);
    DmxSimple.write(20, 0);
    DmxSimple.write(21, 0);
    DmxSimple.write(22, 0);
    DmxSimple.write(23, 0);
    DmxSimple.write(24, 0);
}

static void OnNoteOn(byte channel, byte note, byte velocity)
{
#ifdef ENABLE_SERIAL_DEBUG
    Serial.print(F("NoteOn  from channel: "));
    Serial.print(channel);
    Serial.print(F(", note: "));
    Serial.print(note);
    Serial.print(F(", velocity: "));
    Serial.println(velocity);
#endif

    // Simply map MIDI note number to DMX channels
    if (note > 0 && note <= DMX_NUM_CHAN)
        DmxSimple.write(note, velocity * 2); // * 2 to scale MIDI value (7-bit) to DMX (8-bit)
}

static void OnNoteOff(byte channel, byte note, byte velocity)
{
#ifdef ENABLE_SERIAL_DEBUG
    Serial.print(F("NoteOff from channel: "));
    Serial.print(channel);
    Serial.print(F(", note: "));
    Serial.print(note);
    Serial.print(F(", velocity: "));
    Serial.println(velocity);
#endif

    // Simply map MIDI note number to DMX channels
    if (note > 0 && note <= DMX_NUM_CHAN)
        DmxSimple.write(note, 0); // Just turn off the light
}

static void OnAfterTouchPoly(byte channel, byte note, byte pressure)
{
#ifdef ENABLE_SERIAL_DEBUG
    Serial.print(F("AfterTouchPoly from channel: "));
    Serial.print(channel);
    Serial.print(F(", note: "));
    Serial.print(note);
    Serial.print(F(", pressure: "));
    Serial.println(pressure);
#endif
}

static void OnControlChange(byte channel, byte number, byte value)
{
#ifdef ENABLE_SERIAL_DEBUG
    Serial.print(F("ControlChange from channel: "));
    Serial.print(channel);
    Serial.print(F(", number: "));
    Serial.print(number);
    Serial.print(F(", value: "));
    Serial.println(value);
#endif

    // Simply map MIDI control number to DMX channels
    if (number > 0 && number <= DMX_NUM_CHAN)
        DmxSimple.write(number, value * 2); // * 2 to scale MIDI value (7-bit) to DMX (8-bit)
}

static void OnProgramChange(byte channel, byte number)
{
#ifdef ENABLE_SERIAL_DEBUG
    Serial.print(F("ProgramChange from channel: "));
    Serial.print(channel);
    Serial.print(F(", number: "));
    Serial.println(number);
#endif
}

static void OnAfterTouchChannel(byte channel, byte pressure)
{
#ifdef ENABLE_SERIAL_DEBUG
    Serial.print(F("AfterTouchChannel from channel: "));
    Serial.print(channel);
    Serial.print(F(", pressure: "));
    Serial.println(pressure);
#endif
}

static void OnPitchBend(byte channel, int bend)
{
#ifdef ENABLE_SERIAL_DEBUG
    Serial.print(F("PitchBend from channel: "));
    Serial.print(channel);
    Serial.print(F(", bend: "));
    Serial.println(bend);
#endif
}

static void OnSystemExclusive(byte * array, unsigned size)
{
#ifdef ENABLE_SERIAL_DEBUG
    Serial.println(F("SystemExclusive"));
#endif
}

static void OnTimeCodeQuarterFrame(byte data)
{
#ifdef ENABLE_SERIAL_DEBUG
    Serial.print(F("TimeCodeQuarterFrame: "));
    Serial.println(data, HEX);
#endif
}

static void OnSongPosition(unsigned beats)
{
#ifdef ENABLE_SERIAL_DEBUG
    Serial.print(F("SongPosition: "));
    Serial.println(beats);
#endif
}

static void OnSongSelect(byte songnumber)
{
#ifdef ENABLE_SERIAL_DEBUG
    Serial.print(F("SongSelect: "));
    Serial.println(songnumber);
#endif
}

static void OnTuneRequest()
{
#ifdef ENABLE_SERIAL_DEBUG
    Serial.println(F("TuneRequest"));
#endif
}

static void OnClock()
{
#ifdef ENABLE_SERIAL_DEBUG
    Serial.println(F("Clock"));
#endif
}

static void OnStart()
{
#ifdef ENABLE_SERIAL_DEBUG
    Serial.println(F("Start"));
#endif
}

static void OnContinue()
{
#ifdef ENABLE_SERIAL_DEBUG
    Serial.println(F("Continue"));
#endif
}

static void OnStop()
{
#ifdef ENABLE_SERIAL_DEBUG
    Serial.println(F("Stop"));
#endif

    // Reset all DMX channels on MIDI transport stop
    initDmxChannels();
}

static void OnActiveSensing()
{
#ifdef ENABLE_SERIAL_DEBUG
    Serial.println(F("ActiveSensing"));
#endif
}

static void OnSystemReset()
{
#ifdef ENABLE_SERIAL_DEBUG
    Serial.println(F("SystemReset"));
#endif

    // Reset all DMX channels on MIDI system reset
    initDmxChannels();
}
