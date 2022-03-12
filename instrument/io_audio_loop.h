#ifndef IO_AUDIO_LOOP_H_
#define IO_AUDIO_LOOP_H_

#include <Arduino.h>

template<class AudioCore = void>
class IO_AudioLoop {
   private:
    AudioCore* core;

    // byte currentStep = 0;
    // Step lastStep;

    // void nextOff() {
    //     if (lastStep.duration == 0) {
    //         core->noteOff(lastStep.note);
    //         lastStep.duration = 255;
    //     }
    // }

   public:
    bool active = false;

    IO_AudioLoop(AudioCore* _core) { core = _core; }

    void next() {
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
