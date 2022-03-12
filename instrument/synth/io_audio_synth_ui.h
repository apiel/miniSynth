#ifndef IO_AUDIO_SYNTH_CORE_UI_H_
#define IO_AUDIO_SYNTH_CORE_UI_H_

#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Audio.h>

#include "../../io_display_util.h"
#include "../../io_midi_util.h"
#include "../io_display_helper.h"
#include "./io_audio_synth_core.h"

class IO_AudioSynthCoreUI
{
public:
    IO_AudioSynthCoreUI(IO_AudioSynthCore *_core) { core = _core; }

    void display(Adafruit_SSD1306 *d)
    {
        d->clearDisplay();
        d->setCursor(0, 0);

        d->printf("%s\n", getWave(core->currentWave));
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

    void noteOnHandler(byte channel, byte note, byte velocity)
    {
        core->noteOn(note, velocity);

        // byte key = getItemKey(note);
        // if (key != 255) {
        //     // currentkick = key;
        // } else if (note == 21 || note == 45) {
        // } else if (note == 23 || note == 47) {
        //     // savekick(currentkick);
        // } else if (note == 20) {
        // } else if (note == 17 || note == 41) {
        //     // if (mcMode) {
        //     //     toggleModulation();
        //     // } else {
        //     //     toggleRectifier();
        //     // }
        // }
    }

    void noteOffHandler(byte channel, byte note, byte velocity)
    {
        core->noteOff(note);
    }

    void controlChangeHandler(byte channel, byte control, int8_t direction, byte value)
    {
        if (control == 64)
        {
            // mode = value == 127;
            mode = value != 127;
        }
        else if (mode)
        {
            // bottom row
            if (control == 13)
            {
                core->currentWave = (value / 127.0f) * 13;
                Serial.printf("currentWaveform %d\n", core->currentWave);

                AudioNoInterrupts();
                core->wave.begin(core->currentWave);
                AudioInterrupts();
            }
            else if (control == 14)
            {
                core->wave.offset((value / 127.0f * 2) - 1);
            }
            else if (control == 15)
            {
                core->wave.phase(value / 127.0f * 360.0f);
            }
            else if (control == 16)
            {
                core->wave.pulseWidth(value / 127.0f);
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
            }
        }
        else
        {
            // bottom row
            if (control == 13)
            {
                core->filter.setResonance(value);
            }
            else if (control == 14)
            {
                core->filterLadder.setResonance(value);
            }
            else if (control == 15)
            {
            }
            else if (control == 16)
            {
                core->setRelease(value);
            }
            // top row
            else if (control == 17)
            {
                core->filter.setCutoff(value);
            }
            else if (control == 18)
            {
                core->filterLadder.setCutoff(value);
            }
            else if (control == 19)
            {
                core->setLevel(value);
            }
            else if (control == 20)
            {
                core->setAttack(value);
            }
        }
    }

private:
    IO_AudioSynthCore *core;
    bool mode = false;
};

#endif
