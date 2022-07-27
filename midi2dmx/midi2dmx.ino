#include <USB-MIDI.h>
#include <DmxSimple.h>

#define ON_LED_PIN   13 // Built-in LED, change if using another pin

#define DMX_TX_PIN   1  // Transmit pin, connect to DI of MAX485 module
#define DMX_DE_PIN   10 // Enable pin, connect to DE+RE of MAX485 module

#define MIDI_CHANNEL 1  // MIDI channel to listen to
#define DMX_NUM_CHAN 24 // Number of DMX channels to use, maximum is 512 (if sufficient RAM)

using namespace MIDI_NAMESPACE;

USBMIDI_CREATE_DEFAULT_INSTANCE();

void setup()
{
    // Initialize serial communication
    // Serial.begin(115200);
    // while (!Serial)
    //     ;

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
    // Small RGB PAR
    DmxSimple.write(1, 255); // Dimmer
    DmxSimple.write(2, 0);   // Red
    DmxSimple.write(3, 0);   // Green
    DmxSimple.write(4, 0);   // Blue
    DmxSimple.write(5, 0);   // Strobe
    DmxSimple.write(6, 0);   // Programs

    // Ayra Flash 60 Strobe
    DmxSimple.write(7, 0); // Dimmer
    DmxSimple.write(8, 0); // Strobe

    // ADJ Mega Hex Par
    DmxSimple.write(9, 0);    // Red
    DmxSimple.write(10, 0);   // Green
    DmxSimple.write(11, 0);   // Blue
    DmxSimple.write(12, 0);   // White
    DmxSimple.write(13, 0);   // Amber
    DmxSimple.write(14, 0);   // UV
    DmxSimple.write(15, 255); // Dimmer
    DmxSimple.write(16, 255); // Strobe

    // ADJ Mega Hex Par
    DmxSimple.write(17, 0);   // Red
    DmxSimple.write(18, 0);   // Green
    DmxSimple.write(19, 0);   // Blue
    DmxSimple.write(20, 0);   // White
    DmxSimple.write(21, 0);   // Amber
    DmxSimple.write(22, 0);   // UV
    DmxSimple.write(23, 255); // Dimmer
    DmxSimple.write(24, 255); // Strobe
}

static void OnNoteOn(byte channel, byte note, byte velocity)
{
    // Serial.print(F("NoteOn  from channel: "));
    // Serial.print(channel);
    // Serial.print(F(", note: "));
    // Serial.print(note);
    // Serial.print(F(", velocity: "));
    // Serial.println(velocity);

    // Simply map MIDI note number to DMX channels
    if (note > 0 && note <= DMX_NUM_CHAN)
        DmxSimple.write(note, velocity * 2); // * 2 to scale MIDI value (7-bit) to DMX (8-bit)
}

static void OnNoteOff(byte channel, byte note, byte velocity)
{
    // Serial.print(F("NoteOff from channel: "));
    // Serial.print(channel);
    // Serial.print(F(", note: "));
    // Serial.print(note);
    // Serial.print(F(", velocity: "));
    // Serial.println(velocity);

    // Simply map MIDI note number to DMX channels
    if (note > 0 && note <= DMX_NUM_CHAN)
        DmxSimple.write(note, 0); // Just turn off the light
}

static void OnAfterTouchPoly(byte channel, byte note, byte pressure)
{
    // Serial.print(F("AfterTouchPoly from channel: "));
    // Serial.print(channel);
    // Serial.print(F(", note: "));
    // Serial.print(note);
    // Serial.print(F(", pressure: "));
    // Serial.println(pressure);
}

static void OnControlChange(byte channel, byte number, byte value)
{
    // Serial.print(F("ControlChange from channel: "));
    // Serial.print(channel);
    // Serial.print(F(", number: "));
    // Serial.print(number);
    // Serial.print(F(", value: "));
    // Serial.println(value);

    // Simply map MIDI control number to DMX channels
    if (number > 0 && number <= DMX_NUM_CHAN)
        DmxSimple.write(number, value * 2); // * 2 to scale MIDI value (7-bit) to DMX (8-bit)
}

static void OnProgramChange(byte channel, byte number)
{
    // Serial.print(F("ProgramChange from channel: "));
    // Serial.print(channel);
    // Serial.print(F(", number: "));
    // Serial.println(number);
}

static void OnAfterTouchChannel(byte channel, byte pressure)
{
    // Serial.print(F("AfterTouchChannel from channel: "));
    // Serial.print(channel);
    // Serial.print(F(", pressure: "));
    // Serial.println(pressure);
}

static void OnPitchBend(byte channel, int bend)
{
    // Serial.print(F("PitchBend from channel: "));
    // Serial.print(channel);
    // Serial.print(F(", bend: "));
    // Serial.println(bend);
}

static void OnSystemExclusive(byte * array, unsigned size)
{
    // Serial.println(F("SystemExclusive"));
}

static void OnTimeCodeQuarterFrame(byte data)
{
    // Serial.print(F("TimeCodeQuarterFrame: "));
    // Serial.println(data, HEX);
}

static void OnSongPosition(unsigned beats)
{
    // Serial.print(F("SongPosition: "));
    // Serial.println(beats);
}

static void OnSongSelect(byte songnumber)
{
    // Serial.print(F("SongSelect: "));
    // Serial.println(songnumber);
}

static void OnTuneRequest()
{
    // Serial.println(F("TuneRequest"));
}

static void OnClock()
{
    // Serial.println(F("Clock"));
}

static void OnStart()
{
    // Serial.println(F("Start"));
}

static void OnContinue()
{
    // Serial.println(F("Continue"));
}

static void OnStop()
{
    // Serial.println(F("Stop"));

    // Reset all DMX channels on MIDI transport stop
    initDmxChannels();
}

static void OnActiveSensing()
{
    // Serial.println(F("ActiveSensing"));
}

static void OnSystemReset()
{
    // Serial.println(F("SystemReset"));

    // Reset all DMX channels on MIDI system reset
    initDmxChannels();
}
