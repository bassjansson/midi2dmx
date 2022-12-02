// #define ENABLE_SERIAL_DEBUG // Uncomment this define to enable serial debugging

#define USE_USB_MIDI    // Comment this define to disable use of USB-MIDI
#define USE_SERIAL_MIDI // Comment this define to disable use of DIN-5 Serial MIDI

#ifdef USE_USB_MIDI
    #include <USB-MIDI.h>
#endif
#ifdef USE_SERIAL_MIDI
    #include <MIDI.h>
#endif
#include <DmxSimple.h>


#define ON_LED_PIN   13 // Built-in LED, change if using another pin

#define DMX_TX_PIN   16 // Transmit pin, connect to DI of MAX485 module
#define DMX_DE_PIN   9  // Enable pin, connect to DE+RE of MAX485 module

#define MIDI_CHANNEL MIDI_CHANNEL_OMNI // MIDI channel to listen to, omni means all channels
#define DMX_NUM_CHAN 24                // Number of DMX channels to use, maximum is 512 (if sufficient RAM)


#ifdef USE_USB_MIDI
typedef USBMIDI_NAMESPACE::usbMidiTransport  __umt;
typedef MIDI_NAMESPACE::MidiInterface<__umt> __ss;

__umt usbMIDI(0);                   // cableNr
__ss  MIDICoreUSB((__umt&)usbMIDI); // USB-MIDI
#endif

#ifdef USE_SERIAL_MIDI
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDICoreSerial); // DIN-5 Serial MIDI
#endif


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

#ifdef USE_USB_MIDI
    // Setup all MIDI event callbacks
    MIDICoreUSB.setHandleNoteOn(OnNoteOn);
    MIDICoreUSB.setHandleNoteOff(OnNoteOff);
    MIDICoreUSB.setHandleAfterTouchPoly(OnAfterTouchPoly);
    MIDICoreUSB.setHandleControlChange(OnControlChange);
    MIDICoreUSB.setHandleProgramChange(OnProgramChange);
    MIDICoreUSB.setHandleAfterTouchChannel(OnAfterTouchChannel);
    MIDICoreUSB.setHandlePitchBend(OnPitchBend);
    MIDICoreUSB.setHandleSystemExclusive(OnSystemExclusive);
    MIDICoreUSB.setHandleTimeCodeQuarterFrame(OnTimeCodeQuarterFrame);
    MIDICoreUSB.setHandleSongPosition(OnSongPosition);
    MIDICoreUSB.setHandleSongSelect(OnSongSelect);
    MIDICoreUSB.setHandleTuneRequest(OnTuneRequest);
    MIDICoreUSB.setHandleClock(OnClock);
    MIDICoreUSB.setHandleStart(OnStart);
    MIDICoreUSB.setHandleContinue(OnContinue);
    MIDICoreUSB.setHandleStop(OnStop);
    MIDICoreUSB.setHandleActiveSensing(OnActiveSensing);
    MIDICoreUSB.setHandleSystemReset(OnSystemReset);

    // Listen for MIDI messages on specified channel
    MIDICoreUSB.begin(MIDI_CHANNEL);
#endif

#ifdef USE_SERIAL_MIDI
    // Setup all MIDI event callbacks
    MIDICoreSerial.setHandleNoteOn(OnNoteOn);
    MIDICoreSerial.setHandleNoteOff(OnNoteOff);
    MIDICoreSerial.setHandleAfterTouchPoly(OnAfterTouchPoly);
    MIDICoreSerial.setHandleControlChange(OnControlChange);
    MIDICoreSerial.setHandleProgramChange(OnProgramChange);
    MIDICoreSerial.setHandleAfterTouchChannel(OnAfterTouchChannel);
    MIDICoreSerial.setHandlePitchBend(OnPitchBend);
    MIDICoreSerial.setHandleSystemExclusive(OnSystemExclusive);
    MIDICoreSerial.setHandleTimeCodeQuarterFrame(OnTimeCodeQuarterFrame);
    MIDICoreSerial.setHandleSongPosition(OnSongPosition);
    MIDICoreSerial.setHandleSongSelect(OnSongSelect);
    MIDICoreSerial.setHandleTuneRequest(OnTuneRequest);
    MIDICoreSerial.setHandleClock(OnClock);
    MIDICoreSerial.setHandleStart(OnStart);
    MIDICoreSerial.setHandleContinue(OnContinue);
    MIDICoreSerial.setHandleStop(OnStop);
    MIDICoreSerial.setHandleActiveSensing(OnActiveSensing);
    MIDICoreSerial.setHandleSystemReset(OnSystemReset);

    // Listen for MIDI messages on specified channel
    MIDICoreSerial.begin(MIDI_CHANNEL);
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
#ifdef USE_USB_MIDI
    // Listen to incoming notes
    MIDICoreUSB.read();
#endif

#ifdef USE_SERIAL_MIDI
    // Listen to incoming notes
    MIDICoreSerial.read();
#endif
}

void initDmxChannels()
{
    // Init your lights here.
    // It might be that some of your lights need some channels
    // to be on another state than 0 before they turn on at all.
    // For example, I have some lights which need some channels
    // to be 255 to turn on.

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
