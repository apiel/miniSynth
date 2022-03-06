#ifndef IO_MIDI_H_
#define IO_MIDI_H_

#include <Arduino.h>

#include "io_display.h"
#include "io_instrument.h"
#include "io_midi_core.h"
#include "io_midi_util.h"
#include "io_view.h"

void noteOnHandler(byte channel, byte note, byte velocity)
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

    getSynth()->noteOnHandler(channel, note, velocity);
    displayUpdate();
}

void noteOffHandler(byte channel, byte note, byte velocity)
{
    Serial.print("Note Off, ch=");
    Serial.print(channel, DEC);
    Serial.print(", note=");
    Serial.print(note, DEC);
    Serial.print(", velocity=");
    Serial.println(velocity, DEC);

    getSynth()->noteOffHandler(channel, note, velocity);
    displayUpdate();
}

void controlChangeHandler(byte channel, byte control, byte value)
{
    byte knob = control % KNOB_COUNT;
    Serial.print("Control Change, ch=");
    Serial.print(channel, DEC);
    Serial.print(", control=");
    Serial.print(control, DEC);
    Serial.print(", knob=");
    Serial.print(knob, DEC);
    Serial.print(", value=");
    Serial.println(value, DEC);

    int8_t direction = getKnobDirection(knob, value);
    getSynth()->controlChangeHandler(channel, knob, direction, value);
    displayUpdate();
}

void afterTouchPolyHandler(uint8_t channel, uint8_t note, uint8_t pressure)
{
    Serial.print("AfterTouchPoly, ch=");
    Serial.print(channel, DEC);
    Serial.print(", note=");
    Serial.print(note, DEC);
    Serial.print(", pressure=");
    Serial.println(pressure, DEC);
}

void sysExHandler(const uint8_t *data, uint16_t length, bool complete)
{
    Serial.println("some sysExHandler data");
    // Serial.println(data);
}

void midiInit()
{
    myusb.begin();
    for (byte n = 0; n < MIDI_COUNT; n++)
    {
        midi[n].setHandleNoteOn(noteOnHandler);
        midi[n].setHandleNoteOff(noteOffHandler);
        midi[n].setHandleControlChange(controlChangeHandler);
        midi[n].setHandleAfterTouchPoly(afterTouchPolyHandler);
        midi[n].setHandleSysEx(sysExHandler);
    }
}

void midiLoop()
{
    myusb.Task();
    for (byte n = 0; n < MIDI_COUNT; n++)
    {
        while (midi[n].read())
            ;
    }
}

#endif
