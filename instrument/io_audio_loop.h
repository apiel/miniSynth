#ifndef IO_AUDIO_LOOP_H_
#define IO_AUDIO_LOOP_H_

#include <Arduino.h>

#include "../Pattern.h"
#include "../io_arp_patterns.h"

template <class AudioCore = void>
class IO_AudioLoop
{
private:
    AudioCore *core;

    byte currentStep = 0;
    Step lastStep;

public:
    // instead of active could be modeLoop: true/false.
    // If true, will loop on the last pressed note, if false play only once.
    // There should also be the possibility to set last note to null...
    // bool active = false;
    bool active = true;

    byte currentPattern = 0;
    Pattern *pattern;

    IO_AudioLoop(AudioCore *_core)
    {
        core = _core;
        setCurrentPattern(0);
    }

    void setCurrentPattern(byte value)
    {
        currentPattern = value;
        pattern = &patterns[currentPattern];

        // pattern->print();
    }

    void next()
    {
        if (!lastStep.slide)
        {
            core->noteOff(lastStep.note);
        }
        if (active)
        {

            pattern->print();
            Step *step = &pattern->steps[currentStep];
            if (step->note > 0)
            {
                core->noteOn(step->note, step->velocity);
                lastStep.set(step);
            }
        }
        currentStep = (currentStep + 1) % pattern->stepCount;
    }

    // void toggle() { active = !active; }
    // void activate(bool value = true) { active = value; }
};

#endif
