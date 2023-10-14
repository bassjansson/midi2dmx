#define ENABLE_SERIAL_DEBUG // Comment this define to disable serial debugging

#define USE_USB_MIDI        // Comment this define to disable use of USB-MIDI
#define USE_SERIAL_MIDI     // Comment this define to disable use of DIN-5 Serial MIDI

#ifdef USE_USB_MIDI
    #include <USB-MIDI.h>
#endif
#ifdef USE_SERIAL_MIDI
    #include <MIDI.h>
#endif


#define MIDI_CHANNEL MIDI_CHANNEL_OMNI // MIDI channel to listen to, omni means all channels


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
    // while (!Serial) // Wait for native USB
    //     delay(1);
#endif

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
}
