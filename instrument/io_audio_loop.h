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
    bool active = true;
    bool modeSingleLoop = true;
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

        if (currentStep == 0)
        {
            play = nextToPlay ? nextToPlay : 0;
        }
    }

    void noteOn(byte note)
    {
        if (active)
        {
            nextToPlay = note;
        }
    }

    void noteOff(byte note)
    {
        if (modeSingleLoop && note == nextToPlay)
        {
            nextToPlay = 0;
        }
    }

    void toggle() { activate(!active); }
    void activate(bool value = true) { active = value; }

    void toggleMode() { activateSingleLoopMode(!modeSingleLoop); }
    void activateSingleLoopMode(bool value = true)
    {
        modeSingleLoop = value;
        if (modeSingleLoop) {
            nextToPlay = 0;
        }
    }
};

#endif
