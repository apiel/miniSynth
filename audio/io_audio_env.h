#ifndef IO_AUDIO_ENV_H_
#define IO_AUDIO_ENV_H_

#include <Arduino.h>
#include <Audio.h>

class IO_AudioEnv : public AudioEffectEnvelope
{
public:
    float attackMs = 10.0;
    float releaseMs = 50.0;

    IO_AudioEnv() : AudioEffectEnvelope()
    {
        hold(0);
        attack(attackMs); // ms
        decay(0);
        sustain(1.0);       // level
        release(releaseMs); // ms
    }

    void setAttack(byte value)
    {
        attackMs = 10.0f * value;
        attack(attackMs);
    }

    void setRelease(byte value)
    {
        releaseMs = 20.0f * value;
        release(releaseMs);
    }
};

#endif
