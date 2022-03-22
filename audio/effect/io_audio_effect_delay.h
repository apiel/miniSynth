#ifndef IO_AudioEffectDelay_h_
#define IO_AudioEffectDelay_h_

#include <Arduino.h>
#include <Audio.h>

#include "../audio_dumb.h"

class IO_AudioEffectDelay
{
protected:
    AudioMixer4 mixer;

public:
    AudioEffectDelay input;
    AudioDumb output;

    // need to be public, else no sound
    AudioConnection patches[5] = {
        AudioConnection(input, 0, mixer, 0),
        AudioConnection(input, 1, mixer, 1),
        AudioConnection(input, 2, mixer, 2),
        AudioConnection(input, 3, mixer, 3),
        AudioConnection(mixer, output)};

    IO_AudioEffectDelay(void)
    {
        input.delay(0, 110);
        input.delay(1, 220);
        input.delay(2, 330);
    }
};

#endif
