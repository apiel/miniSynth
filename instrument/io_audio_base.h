#ifndef IO_AUDIO_BASE_H_
#define IO_AUDIO_BASE_H_

#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Audio.h>

#include "./io_audio_loop.h"

template <class AudioCore = void, class AudioCoreUI = void>
class IO_AudioBase
{
protected:
public:
    AudioCoreUI *coreUI;
    IO_AudioLoop<AudioCore> *loop;

    byte currentPattern = 0;
    Pattern *pattern;

    // for the moment will always be arpMode
    bool arpMode = true;

    void init() {}

    void display(Adafruit_SSD1306 *d, unsigned int * forceRefreshIn)
    {
        coreUI->display(d, forceRefreshIn);
    }

    void noteOnHandler(byte channel, byte note, byte velocity)
    {
        if (channel == 10) {
            if (note == 36) {
                loop->toggleMode();
            }
            return;
        }

        if (arpMode)
        {
            loop->noteOn(note);
        }
        else
        {
            coreUI->noteOnHandler(channel, note, velocity);
        }
    }

    void noteOffHandler(byte channel, byte note, byte velocity)
    {
         if (channel == 10) {
            return;
        }

        if (arpMode)
        {
            loop->noteOff(note);
        }
        else
        {
            coreUI->noteOffHandler(channel, note, velocity);
        }
    }

    void controlChangeHandler(byte channel, byte control, int8_t direction, byte value)
    {
        coreUI->controlChangeHandler(channel, control, direction, value);
    }
};

#endif
