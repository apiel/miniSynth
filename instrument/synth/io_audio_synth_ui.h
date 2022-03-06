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
private:
    IO_AudioSynthCore *core;

public:
    IO_AudioSynthCoreUI(IO_AudioSynthCore *_core) { core = _core; }

    void display(Adafruit_SSD1306 *d)
    {
        d->clearDisplay();
        d->setCursor(0, 0);

        d->printf("%s %.1fHz\n", getWave(core->currentWave), core->frequency);
        d->printf("%d%% %d|%d|%d%%|%d\n", (int)(core->amplitude * 100.0),
                  (int)core->adsr[0], (int)core->adsr[1],
                  (int)(core->adsr[2] * 100.0), (int)core->adsr[3]);

        addToCursor(d, 0, 4);
        d->printf("%s %.1fHz %d\n", getFilter(core->filter.currentFilter),
                  core->filter.filterFrequency, core->filter.filterResonance);
        d->printf("%.1f %d|%d|%d%%|%d\n", core->filter.dcValue,
                  (int)core->filter.adsr[0], (int)core->filter.adsr[1],
                  (int)(core->filter.adsr[2] * 100.0),
                  (int)core->filter.adsr[3]);
        d->printf("Dist %d range %d\n", (int)core->distortion.amount,
                  (int)core->distortion.range);
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

    void controlChangeHandler(byte channel, byte knob, int8_t direction)
    {
        if (channel == 11)
        {
            // if (knob == 1) {
            //     if (mcMode) {
            //         core->filter.setCurrentFilter(direction);
            //     } else {
            //         core->setNextWave(direction);
            //     }
            // } else if (knob == 2) {
            //     if (mcMode) {
            //         core->filter.setFilterFrequency(direction);
            //     } else {
            //         core->setFrequency(direction);
            //     }
            // } else if (knob == 3) {
            //     if (mcMode) {
            //         core->filter.setFilterResonance(direction);
            //     } else {
            //         core->setAmplitude(direction);
            //     }
            // } else if (knob == 4) {
            //     if (mcMode) {
            //         core->filter.setDc(direction);
            //     } else {
            //     }
            // } else if (knob == 5) {
            //     if (mcMode) {
            //         core->filter.setAttack(direction);
            //     } else {
            //         core->setAttack(direction);
            //     }
            // } else if (knob == 6) {
            //     if (mcMode) {
            //         core->filter.setDecay(direction);
            //     } else {
            //         core->setDecay(direction);
            //     }
            // } else if (knob == 7) {
            //     if (mcMode) {
            //         core->filter.setSustain(direction);
            //     } else {
            //         core->setSustain(direction);
            //     }
            // } else if (knob == 8) {
            //     if (mcMode) {
            //         core->filter.setRelease(direction);
            //     } else {
            //         core->setRelease(direction);
            //     }
            // } else if (knob == 11) {
            //     if (mcMode) {
            //         core->setDistortion(direction);
            //     } else {
            //     }
            // } else if (knob == 12) {
            //     if (mcMode) {
            //         core->setDistortionRange(direction);
            //     } else {
            //     }
            // } else if (knob == 13) {
            //     if (mcMode) {
            //     } else {
            //     }
            // } else if (knob == 14) {
            //     if (mcMode) {
            //     } else {
            //     }
            // } else if (knob == 15) {
            //     if (mcMode) {
            //     } else {
            //     }
            // } else if (knob == 16) {
            //     if (mcMode) {
            //     } else {
            //     }
            // } else if (knob == 17) {
            //     if (mcMode) {
            //     } else {
            //     }
            // } else if (knob == 0) {  // 0 for 18
            //     if (mcMode) {
            //     } else {
            //     }
            // }
        }
    }
};

#endif
