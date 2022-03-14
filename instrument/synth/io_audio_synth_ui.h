#ifndef IO_AUDIO_SYNTH_CORE_UI_H_
#define IO_AUDIO_SYNTH_CORE_UI_H_

#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Audio.h>

#include "../../io_display_util.h"
#include "../../io_midi_util.h"
#include "./io_audio_synth_core.h"

class IO_AudioSynthCoreUI
{
public:
    IO_AudioSynthCoreUI(IO_AudioSynthCore *_core) { core = _core; }

    void display(Adafruit_SSD1306 *d, unsigned int * forceRefreshIn)
    {
        d->clearDisplay();
        d->setCursor(0, 0);

        if (displayValName)
        {
            d->println(displayValName);
            d->println("");
            d->setTextSize(6);
            d->println(displayVal);
            d->setTextSize(1);
            displayValName = NULL;
            *forceRefreshIn = 1000;
        }
        else
        {
            d->printf("%s\n", core->wave.getName());
            // d->printf("%d%% %d|%d|%d%%|%d\n", (int)(core->amplitude * 100.0),
            //           (int)core->adsr[0], (int)core->adsr[1],
            //           (int)(core->adsr[2] * 100.0), (int)core->adsr[3]);

            addToCursor(d, 0, 4);
            // d->printf("%s %.1fHz %d\n", getFilter(core->filter.currentFilter),
            //           core->filter.filterFrequency, core->filter.filterResonance);
            // d->printf("%.1f %d|%d|%d%%|%d\n", core->filter.dcValue,
            //           (int)core->filter.adsr[0], (int)core->filter.adsr[1],
            //           (int)(core->filter.adsr[2] * 100.0),
            //           (int)core->filter.adsr[3]);
            // d->printf("Dist %d range %d\n", (int)core->distortion.amount,
            //           (int)core->distortion.range);
        }
    }

    void displayValue(const char * name, byte value)
    {
        displayVal = value;
        displayValName = name;
    }

    void noteOnHandler(byte channel, byte note, byte velocity)
    {
        core->noteOn(note, velocity);
    }

    void noteOffHandler(byte channel, byte note, byte velocity)
    {
        core->noteOff(note);
    }

    void controlChangeHandler(byte channel, byte control, int8_t direction, byte value)
    {
        if (control == 64)
        {
            mode = value == 127;
            // mode = value != 127;
        }
        else if (mode)
        {
            // bottom row
            if (control == 13)
            {
                core->wave.setWaveform(value);
            }
            else if (control == 14)
            {
                core->wave.edit(value);
                displayValue("Wave edit", value);
            }
            else if (control == 15)
            {
            }
            else if (control == 16)
            {
            }
            // top row
            else if (control == 17)
            {
            }
            else if (control == 18)
            {
            }
            else if (control == 19)
            {
            }
            else if (control == 20)
            {
                core->setLevel(value);
                displayValue("Level", value);
            }
        }
        else
        {
            // bottom row
            if (control == 13)
            {
                core->filter.setResonance(value);
                displayValue("Resonnance", value);
            }
            else if (control == 14)
            {
                core->filterLadder.setResonance(value);
                displayValue("Ladder Resonnance", value);
            }
            else if (control == 15)
            {
            }
            else if (control == 16)
            {
                core->env.setRelease(value);
            }
            // top row
            else if (control == 17)
            {
                core->filter.setCutoff(value);
                displayValue("Cutoff", value);
            }
            else if (control == 18)
            {
                core->filterLadder.setCutoff(value);
                displayValue("Ladder Cutoff", value);
            }
            else if (control == 19)
            {
            }
            else if (control == 20)
            {
                core->env.setAttack(value);
            }
        }
    }

private:
    IO_AudioSynthCore *core;
    bool mode = false;
    byte displayVal = 0;
    const char * displayValName = NULL;
};

#endif
