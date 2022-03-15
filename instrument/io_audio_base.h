#ifndef IO_AUDIO_BASE_H_
#define IO_AUDIO_BASE_H_

#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Audio.h>

#include "./io_audio_loop.h"
#include "../io_instrument.h"

template <class AudioCore = void, class AudioCoreUI = void>
class IO_AudioBase
{
protected:
    byte displayVal = 0;
    const char *displayValName = NULL;

    unsigned int msVal = 0;
    const char *msValName = NULL;

    IO_AudioLoop<AudioCore> *loopPadPressed = NULL;
    bool loopPadPressedDidAction = false;

public:
    byte id = 0;
    AudioCoreUI *coreUI;
    IO_AudioLoop<AudioCore> *loop;

    byte currentPattern = 0;
    Pattern *pattern;

    // for the moment will always be arpMode
    bool arpMode = true;

    void init(byte _id) { id = _id; }

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
                loopPadPressed = getSynth(0)->loop;
            }
            else if (note == 37)
            {
                loopPadPressed = getSynth(1)->loop;
            }
            else if (note == 38)
            {
                loopPadPressed = getSynth(2)->loop;
            }
            else if (note == 39)
            {
                loopPadPressed = getSynth(3)->loop;
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

    void loopPadPressedAction(byte _id)
    {
        if (_id != id)
        {
            setCurrentSynth(_id);
        }
        else if (!loopPadPressedDidAction)
        {
            loop->toggleMode();
        }
        loopPadPressed = NULL;
        loopPadPressedDidAction = false;
    }

    void noteOffHandler(byte channel, byte note, byte velocity)
    {
        if (channel == 10)
        {
            if (note == 36)
            {
                loopPadPressedAction(0);
            }
            else if (note == 37)
            {
                loopPadPressedAction(1);
            }
            else if (note == 38)
            {
                loopPadPressedAction(2);
            }
            else if (note == 39)
            {
                loopPadPressedAction(3);
            }
            else if (note == 40)
            {
                loop->setCurrentPatternSelector(0);
                displayValue("Pattern selector 0", loop->patternSelector[0]);
            }
            else if (note == 41)
            {
                loop->setCurrentPatternSelector(1);
                displayValue("Pattern selector 1", loop->patternSelector[1]);
            }
            else if (note == 42)
            {
                loop->setCurrentPatternSelector(2);
                displayValue("Pattern selector 2", loop->patternSelector[2]);
            }
            else if (note == 43)
            {
                loop->setCurrentPatternSelector(3);
                displayValue("Pattern selector 3", loop->patternSelector[3]);
            }
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
        if (loopPadPressed)
        {
            if (control == 17)
            {
                loopPadPressed->setPatternSelector(0, value);
                displayValue("Pattern selector 0", loopPadPressed->patternSelector[0]);
                loopPadPressedDidAction = true;
            }
            else if (control == 18)
            {
                loopPadPressed->setPatternSelector(1, value);
                displayValue("Pattern selector 1", loopPadPressed->patternSelector[1]);
                loopPadPressedDidAction = true;
            }
            else if (control == 19)
            {
                loopPadPressed->setPatternSelector(2, value);
                displayValue("Pattern selector 2", loopPadPressed->patternSelector[2]);
                loopPadPressedDidAction = true;
            }
            else if (control == 20)
            {
                loopPadPressed->setPatternSelector(3, value);
                displayValue("Pattern selector 3", loopPadPressed->patternSelector[3]);
                loopPadPressedDidAction = true;
            }
            return;
        }
        coreUI->controlChangeHandler(channel, control, direction, value);
    }
};

#endif
