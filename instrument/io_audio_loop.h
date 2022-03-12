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

    // byte currentStep = 0;
    // Step lastStep;

    // void nextOff() {
    //     if (lastStep.duration == 0) {
    //         core->noteOff(lastStep.note);
    //         lastStep.duration = 255;
    //     }
    // }

public:
    // instead of active could be modeLoop: true/false.
    // If true, will loop on the last pressed note, if false play only once.
    // There should also be the possibility to set last note to null...
    bool active = false;

    byte currentPattern = 0;
    Pattern *pattern;

    IO_AudioLoop(AudioCore *_core) { core = _core; }

    void init() { setCurrentPattern(0); }

    void setCurrentPattern(byte value)
    {
        currentPattern = value;
        pattern = &patterns[currentPattern];
    }

    void next()
    {
        // if (lastStep.duration != 255) {
        //     lastStep.duration--;
        //     if (!lastStep.slide) {
        //         nextOff();
        //     }
        // }
        // if (active) {
        //     Step* step = &pattern->steps[currentStep];
        //     if (step->duration) {
        //         core->noteOn(step->note, step->velocity);
        //         if (lastStep.duration != 255) {
        //             lastStep.duration = 0;
        //             nextOff();
        //         }
        //         lastStep.set(step);
        //     }
        // }
        // // In case lastStep is a slide but no current step was set
        // // Still need to noteOff
        // nextOff();
        // currentStep = (currentStep + 1) % pattern->stepCount;
    }

    // void toggle() { active = !active; }
    // void activate(bool value = true) { active = value; }
};

#endif
