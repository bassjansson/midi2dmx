#include <USB-MIDI.h>
#include <DmxSimple.h>

#define MIDI_CHANNEL 1
#define DMX_TX_PIN   1
#define DMX_DE_PIN   2
#define DMX_NUM_CHAN 24

using namespace MIDI_NAMESPACE;

USBMIDI_CREATE_DEFAULT_INSTANCE();

void setup()
{
    // Initialize serial communication
    // Serial.begin(115200);
    // while (!Serial)
    //     ;

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

    // Init some lights
    DmxSimple.write(8 + 7, 255);
    DmxSimple.write(8 + 8, 255);
    DmxSimple.write(16 + 7, 255);
    DmxSimple.write(16 + 8, 255);
} // setup

void loop()
{
    // Listen to incoming notes
    MIDI.read();
}

static void OnNoteOn(byte channel, byte note, byte velocity)
{
    // Serial.print(F("NoteOn  from channel: "));
    // Serial.print(channel);
    // Serial.print(F(", note: "));
    // Serial.print(note);
    // Serial.print(F(", velocity: "));
    // Serial.println(velocity);

    if (note > 0 && note <= DMX_NUM_CHAN)
        DmxSimple.write(note, velocity * 2);
}

static void OnNoteOff(byte channel, byte note, byte velocity)
{
    // Serial.print(F("NoteOff from channel: "));
    // Serial.print(channel);
    // Serial.print(F(", note: "));
    // Serial.print(note);
    // Serial.print(F(", velocity: "));
    // Serial.println(velocity);

    if (note > 0 && note <= DMX_NUM_CHAN)
        DmxSimple.write(note, 0);
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

    if (number > 0 && number <= DMX_NUM_CHAN)
        DmxSimple.write(number, value * 2);
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
}

static void OnActiveSensing()
{
    // Serial.println(F("ActiveSensing"));
}

static void OnSystemReset()
{
    // Serial.println(F("SystemReset"));
}
