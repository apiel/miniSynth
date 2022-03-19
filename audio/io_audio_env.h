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
        attackMs = 5.0f * value;
        // Serial.printf("setAttack %.6f\n", attackMs);
        attack(attackMs);
    }

    void setRelease(byte value)
    {
        releaseMs = 1.0f * value * value;
        // Serial.printf("setRelease %.6f\n", releaseMs);
        release(releaseMs);
    }

    void noteOn(byte note, byte velocity)
    {
        Serial.println("note on in IO_AudioEnv");
        AudioEffectEnvelope::noteOn();
    }

    void noteOff(byte note)
    {
        AudioEffectEnvelope::noteOff();
    }
};

#endif
