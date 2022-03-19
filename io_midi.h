#ifndef IO_MIDI_H_
#define IO_MIDI_H_

#include <Arduino.h>
#include <USBHost_t36.h>

#include "io_display.h"
#include "controller/io_controller_akai_mpk_mini.h"

#define MIDI_COUNT 4

USBHost myusb;
USBHub hub1(myusb);
USBHub hub2(myusb);
USBHub hub3(myusb);
MIDIDevice midi[MIDI_COUNT] = MIDIDevice(myusb);

void noteOnHandler(byte channel, byte note, byte velocity);
void noteOffHandler(byte channel, byte note, byte velocity);
void controlChangeHandler(byte channel, byte control, byte value);

class IO_Midi
{
protected:
    IO_ControllerAkaiMPKmini *controller;

public:
    void init(IO_ControllerAkaiMPKmini *_controller)
    {
        controller = _controller;

        myusb.begin();
        for (byte n = 0; n < MIDI_COUNT; n++)
        {
            midi[n].setHandleNoteOn(noteOnHandler);
            midi[n].setHandleNoteOff(noteOffHandler);
            midi[n].setHandleControlChange(controlChangeHandler);
            // midi[n].setHandleSysEx(sysExHandler);
        }
    }

    void loop()
    {
        myusb.Task();
        for (byte n = 0; n < MIDI_COUNT; n++)
        {
            while (midi[n].read())
                ;
        }
    }

    void noteOn(byte channel, byte note, byte velocity)
    {
        // When a USB device with multiple virtual cables is used,
        // midi[n].getCable() can be used to read which of the virtual
        // MIDI cables received this message.
        Serial.print("Note On, ch=");
        Serial.print(channel, DEC);
        Serial.print(", note=");
        Serial.print(note, DEC);
        Serial.print(", velocity=");
        Serial.println(velocity, DEC);

        controller->noteOnHandler(channel, note, velocity);
        display.update();
    }

    void noteOff(byte channel, byte note, byte velocity)
    {
        Serial.print("Note Off, ch=");
        Serial.print(channel, DEC);
        Serial.print(", note=");
        Serial.print(note, DEC);
        Serial.print(", velocity=");
        Serial.println(velocity, DEC);

        controller->noteOffHandler(channel, note, velocity);
        display.update();
    }

    void controlChange(byte channel, byte control, byte value)
    {
        Serial.print("Control Change, ch=");
        Serial.print(channel, DEC);
        Serial.print(", control=");
        Serial.print(control, DEC);
        Serial.print(", value=");
        Serial.println(value, DEC);

        controller->controlChangeHandler(channel, control, value);
        display.update();
    }

    // void sysExHandler(const uint8_t *data, uint16_t length, bool complete)
    // {
    //     Serial.println("some sysExHandler data");
    //     // Serial.println(data);
    // }
};

IO_Midi ioMidi;

void noteOnHandler(byte channel, byte note, byte velocity)
{
    ioMidi.noteOn(channel, note, velocity);
}

void noteOffHandler(byte channel, byte note, byte velocity)
{
    ioMidi.noteOff(channel, note, velocity);
}

void controlChangeHandler(byte channel, byte control, byte value)
{
    ioMidi.controlChange(channel, control, value);
}

#endif
