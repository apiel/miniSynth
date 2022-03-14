#ifndef IO_AUDIO_LOOP_H_
#define IO_AUDIO_LOOP_H_

#include <Arduino.h>

#include "../Pattern.h"
#include "../io_arp_patterns.h"

#define REF_NOTE _C4

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
    bool active = true;
    // bool play = false;
    // bool modeLoop = false;
    byte nextToPlay = 0;
    byte play = 0;

    byte currentPattern = 2;
    Pattern *pattern;

    IO_AudioLoop(AudioCore *_core)
    {
        core = _core;
        setCurrentPattern(currentPattern);
    }

    void setCurrentPattern(byte value)
    {
        currentPattern = value;
        pattern = &patterns[currentPattern];
    }

    void next()
    {
        if (!lastStep.slide)
        {
            core->noteOff(lastStep.note);
            // to avoid repeating this again, let set slide to true
            lastStep.slide = true;
        }

        if (play)
        {
            Step *step = &pattern->steps[currentStep];
            if (step->note > 0)
            {
                lastStep.set(step);
                // add note difference to note
                lastStep.note += (int)play - (int)REF_NOTE;
                core->noteOn(lastStep.note, lastStep.velocity);
            }
        }
        currentStep = (currentStep + 1) % pattern->stepCount;

        if (/* modeLoop  && */ currentStep == 0)
        {
            play = nextToPlay ? nextToPlay : 0;
        }
    }

    void noteOn(byte note)
    {
        if (active)
        {
            // if (modeLoop)
            // {
            // }
            // else
            // {
            Serial.printf("nextToPlay %d\n", note);

            nextToPlay = note;
            // }
        }
    }

    void noteOff(byte note)
    {
        if (note == nextToPlay)
        {
            nextToPlay = 0;
        }
    }

    // void toggle() { active = !active; }
    // void activate(bool value = true) { active = value; }
};

#endif
