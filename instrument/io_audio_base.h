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
    byte displayVal = 0;
    const char *displayValName = NULL;

    unsigned int msVal = 0;
    const char *msValName = NULL;

public:
    AudioCoreUI *coreUI;
    IO_AudioLoop<AudioCore> *loop;

    byte currentPattern = 0;
    Pattern *pattern;

    // for the moment will always be arpMode
    bool arpMode = true;

    void init() {}

    void display(Adafruit_SSD1306 *d, unsigned int *forceRefreshIn)
    {
        d->clearDisplay();
        d->setCursor(0, 0);

        if (displayValName) // should move this in io_audio_base
        {
            d->println(displayValName);
            d->println("");
            d->setTextSize(6);
            d->println(displayVal);
            d->setTextSize(1);
            displayValName = NULL;
            *forceRefreshIn = 2000;
        }
        else if (msValName) // should move this in io_audio_base
        {
            d->println(msValName);
            d->setTextSize(3);
            d->printf("\n%dms\n", msVal);
            d->setTextSize(1);
            msValName = NULL;
            *forceRefreshIn = 2000;
        }
        else
        {
            coreUI->display(d, forceRefreshIn);
        }
    }

    void displayValue(const char *name, byte value)
    {
        displayVal = value;
        displayValName = name;
    }

    void displayMs(const char *name, unsigned int value)
    {
        msVal = value;
        msValName = name;
    }

    void noteOnHandler(byte channel, byte note, byte velocity)
    {
        if (channel == 10)
        {
            if (note == 36)
            {
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
        if (channel == 10)
        {
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
