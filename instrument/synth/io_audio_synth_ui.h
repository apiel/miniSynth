#ifndef IO_AUDIO_SYNTH_CORE_UI_H_
#define IO_AUDIO_SYNTH_CORE_UI_H_

#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Audio.h>

#include "../../io_display_util.h"
#include "../../io_midi_util.h"

#include "../io_audio_base.h"
#include "./io_audio_synth.h"

class IO_AudioSynthCoreUI
{
public:
    IO_AudioSynthCoreUI(IO_AudioSynthCore * _core, IO_AudioBase<IO_AudioSynthCore, IO_AudioSynthCoreUI> *_base)
    {
        core = _core;
        base = _base;
    }

    void display(Adafruit_SSD1306 *d, unsigned int *forceRefreshIn)
    {

        d->printf("%d %s\n", core->wave.currentWave, core->wave.getName());
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

// Feel like control are too coupled to the synth and arp system...
// maybe this whole stuff should extracted!
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
                base->displayValue("Wave edit", value);
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
                base->displayValue("Level", value);
            }
        }
        else
        {
            // bottom row
            if (control == 13)
            {
                core->filter.setResonance(value);
                base->displayValue("Resonnance", value);
            }
            else if (control == 14)
            {
                core->filterLadder.setResonance(value);
                base->displayValue("Ladder Resonnance", value);
            }
            else if (control == 15)
            {
            }
            else if (control == 16)
            {
                core->setRelease(value);
                base->displayMs("Env. Release", core->releaseMs);
            }
            // top row
            else if (control == 17)
            {
                core->filter.setCutoff(value);
                base->displayValue("Cutoff", value);
            }
            else if (control == 18)
            {
                core->filterLadder.setCutoff(value);
                base->displayValue("Ladder Cutoff", value);
            }
            else if (control == 19)
            {
            }
            else if (control == 20)
            {
                core->setAttack(value);
                base->displayMs("Env. Attack", core->attackMs);
            }
        }
    }

private:
    IO_AudioSynthCore *core;
    IO_AudioBase<IO_AudioSynthCore, IO_AudioSynthCoreUI> *base;
    bool mode = false;
};

#endif
