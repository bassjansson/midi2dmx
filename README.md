# MIDI2DMX

[Video Tutorial on YouTube :)](https://youtu.be/dqrqfa0cMyE)

## USB-MIDI to DMX-out

Firmware for a tiny but powerful tool which acts as a USB-MIDI device just like any modern synthesizer, but it doesn't make any sounds, it sends the MIDI messages as DMX values to your DMX lights system instead!

Now you will not need these big and expensive DMX modules anymore and you'll never have to leave your DAW again to some other app to program your light show! Sync your beats with your lights and create a lightshow simply using MIDI notes and MIDI control changes. Need more control? Update the firmware with your custom code or create an Ableton Max for Live patch to get even more out of it!

Enjoy! :)

## What hardware do I need?

- An **ATmega32U4** microcontroller, for example:
    - Arduino Leonardo (https://docs.arduino.cc/hardware/leonardo)
    - Arduino Micro (https://store.arduino.cc/products/arduino-micro)
    - Adafruit ItsyBitsy (https://www.adafruit.com/product/3677)
    - DFRobot Beetle (https://www.dfrobot.com/product-1075.html)
- A **MAX485** module ([amazon.com](https://www.amazon.com/Max485-Chip-RS-485-Module-Raspberry/dp/B00NIOLNAG/ref=sr_1_3?crid=2LWYPWLXT9T3W&keywords=max485&qid=1658936062&sprefix=max48%2Caps%2C170&sr=8-3))
- An **XLR-female** connector/panelmount or a short XLR cable to cut in half
- A **Micro-USB** cable
- Some (jumper) cables

**NOTE:** Other Arduino microcontrollers are not tested. You will _NEED_ a microcontroller which has native-USB support, just like the 32U4, otherwise the USB-MIDI won't work. There are workarounds e.g. for the Arduino Uno, but I won't get into detail on that here.

## What software do I need?

- [Arduino IDE](https://www.arduino.cc/en/software)
- [Arduino MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library)
- [Arduino MIDIUSB Library](https://github.com/arduino-libraries/MIDIUSB)
- [Arduino USB-MIDI Library](https://github.com/lathoub/Arduino-USBMIDI)
- [Arduino DmxSimple Library](https://github.com/PaulStoffregen/DmxSimple)

## How to upload the code?

Use the Arduino IDE and select **Arduino Leonardo** as the board, doesn't matter which 32U4 board you have from the list of boards above.

## Schematic

![schematic](./images/schematic.jpg)

## Finished Device

![device](./images/device.jpg)

## You're welcome!

Bass Jansson, Technology for the Arts
