#ifndef IO_AudioEffectDelay_h_
#define IO_AudioEffectDelay_h_

#include <Arduino.h>
#include <Audio.h>

#include "../audio_dumb.h"

class IO_AudioEffectDelay
{
protected:
    AudioEffectDelay delay;
    AudioMixer4 mixer;

    AudioConnection patches[6] = {
        AudioConnection(input, delay),
        AudioConnection(delay, 0, mixer, 0),
        AudioConnection(delay, 1, mixer, 1),
        AudioConnection(delay, 2, mixer, 2),
        AudioConnection(delay, 3, mixer, 3),
        AudioConnection(mixer, output)};

public:
    AudioDumb input;
    AudioDumb output;

    IO_AudioEffectDelay(void)
    {
        delay.delay(0, 110);
        delay.delay(1, 220);
        delay.delay(2, 330);
    }
};

#endif
