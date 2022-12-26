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

struct RgbColor
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

uint8_t dmxTargetBuff[DMX_NUM_CHAN];
uint8_t dmxCurrentBuff[DMX_NUM_CHAN];

uint8_t lastNoteNumber   = 21; // default to color magenta
uint8_t lastNoteVelocity = 63; // default to half brightness

float colorDimmer = 1.0f;
float colorBend   = 0.0f;

int fadeNegInterval  = 5; // interval in ms
int fadePosIncrement = 2; // increment

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

    // Copy target buff values to the current one for a smooth startup
    for (int i = 0; i < DMX_NUM_CHAN; ++i)
        dmxCurrentBuff[i] = dmxTargetBuff[i];
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

    // Update DMX values by notes played
    updateDmxByNotes();

    // Send DMX values to lights
    dmxUpdate();
}

void dmxWrite(uint16_t channel, uint8_t value)
{
    uint16_t c = channel - 1;

    if (c < DMX_NUM_CHAN)
        dmxTargetBuff[c] = value;
}

void dmxUpdate()
{
    // This method fades DMX target buffer to DMX current buffer
    // and then writes DMX current buffer to the MAX485 chip.

    static unsigned long fadeTime = 0;

    if (millis() >= fadeTime)
    {
        fadeTime = millis() + fadeNegInterval;

        for (int i = 0; i < DMX_NUM_CHAN; ++i)
        {
            int16_t diff = (int16_t)dmxTargetBuff[i] - (int16_t)dmxCurrentBuff[i];

            if (diff > 0)
                dmxCurrentBuff[i] += diff > fadePosIncrement ? fadePosIncrement : diff;
            else if (diff < 0)
                dmxCurrentBuff[i]--;

            DmxSimple.write(i + 1, dmxCurrentBuff[i]);
        }
    }
}

void initDmxChannels()
{
    // Init your lights here.
    // It might be that some of your lights need some channels
    // to be on another state than 0 before they turn on at all.
    // For example, I have some lights which need some channels
    // to be 255 to turn on.

    // RGB Spot
    dmxWrite(1, 255);
    dmxWrite(2, 0); // R
    dmxWrite(3, 0); // G
    dmxWrite(4, 0); // B
    dmxWrite(5, 0);
    dmxWrite(6, 0);

    // White Strobe
    dmxWrite(7, 0); // W
    dmxWrite(8, 0);

    // Hex Washer 1
    dmxWrite(9, 0);  // R
    dmxWrite(10, 0); // G
    dmxWrite(11, 0); // B
    dmxWrite(12, 0); // W
    dmxWrite(13, 0); // A
    dmxWrite(14, 0); // UV
    dmxWrite(15, 255);
    dmxWrite(16, 255);

    // Hex Washer 2
    dmxWrite(17, 0); // R
    dmxWrite(18, 0); // G
    dmxWrite(19, 0); // B
    dmxWrite(20, 0); // W
    dmxWrite(21, 0); // A
    dmxWrite(22, 0); // UV
    dmxWrite(23, 255);
    dmxWrite(24, 255);
}

void writeRgb(RgbColor rgb)
{
    // Spot
    dmxWrite(2, rgb.r); // R
    dmxWrite(3, rgb.g); // G
    dmxWrite(4, rgb.b); // B

    // Washer 1
    dmxWrite(9, rgb.r);  // R
    dmxWrite(10, rgb.g); // G
    dmxWrite(11, rgb.b); // B

    // Washer 2
    dmxWrite(17, rgb.r); // R
    dmxWrite(18, rgb.g); // G
    dmxWrite(19, rgb.b); // B
}

RgbColor hsl2rgb(uint8_t hue, uint8_t saturation, uint8_t lightness)
{
    RgbColor rgb;

    if (saturation == 0)
    {
        rgb.r = lightness;
        rgb.g = lightness;
        rgb.b = lightness;

        return rgb;
    }

    uint16_t h = hue;
    uint16_t s = saturation;
    uint16_t v = lightness;
    uint8_t  a = h / 43;
    uint16_t m = (h - (a * 43)) * 6;
    uint8_t  p = (v * (255 - s)) >> 8;
    uint8_t  q = (v * (255 - ((s * m) >> 8))) >> 8;
    uint8_t  t = (v * (255 - ((s * (255 - m)) >> 8))) >> 8;

    switch (a)
    {
        case 0:
            rgb.r = v;
            rgb.g = t;
            rgb.b = p;
            break;
        case 1:
            rgb.r = q;
            rgb.g = v;
            rgb.b = p;
            break;
        case 2:
            rgb.r = p;
            rgb.g = v;
            rgb.b = t;
            break;
        case 3:
            rgb.r = p;
            rgb.g = q;
            rgb.b = v;
            break;
        case 4:
            rgb.r = t;
            rgb.g = p;
            rgb.b = v;
            break;
        default:
            rgb.r = v;
            rgb.g = p;
            rgb.b = q;
            break;
    }

    return rgb;
}

void updateDmxByNotes()
{
    static uint8_t hue   = 0;
    static uint8_t light = 0;
    static uint8_t white = 0;

    // First two octaves is color wheel
    if (lastNoteNumber < 24)
    {
        hue   = lastNoteNumber / 24.0f * 256.0f;
        light = lastNoteVelocity + 128;
        // white = 0;
    }
    // Third octave is white light dimmer
    else if (lastNoteNumber < 36)
    {
        // light = 0;
        white = (lastNoteNumber - 24) / 11.0f * 255.0f;
    }
    // Fourth octave, first half is all lights off
    else if (lastNoteNumber < 41)
    {
        light = 0;
        white = 0;
    }
    // Fourth octave, second half is light fading speed
    else if (lastNoteNumber < 49)
    {
        fadeNegInterval  = lastNoteNumber - 41;
        fadePosIncrement = 2;
    }

    // Write color lights
    writeRgb(hsl2rgb(hue + colorBend * 32.0f, 255, light * colorDimmer));

    // Write white lights
    uint8_t w = white * colorDimmer;
    dmxWrite(7, w);  // Strobe White
    dmxWrite(13, w); // Amber Washer 1
    dmxWrite(21, w); // Amber Washer 2
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

    lastNoteNumber   = note - 36;
    lastNoteVelocity = velocity;

    // Simply map MIDI note number to DMX channels
    // if (note > 0 && note <= DMX_NUM_CHAN)
    //     dmxWrite(note, velocity * 2); // * 2 to scale MIDI value (7-bit) to DMX (8-bit)
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
    // if (note > 0 && note <= DMX_NUM_CHAN)
    //     dmxWrite(note, 0); // Just turn off the light
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

    if (number == 1)
        colorDimmer = value / 127.0f;

    // Simply map MIDI control number to DMX channels
    // if (number > 0 && number <= DMX_NUM_CHAN)
    //     dmxWrite(number, value * 2); // * 2 to scale MIDI value (7-bit) to DMX (8-bit)
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

    colorBend = bend / 8192.0f;
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
