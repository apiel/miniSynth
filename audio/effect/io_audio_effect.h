#ifndef IO_AUDIO_EFFECT_H_
#define IO_AUDIO_EFFECT_H_

#include <Arduino.h>
#include <Audio.h>

#include "AudioEffectDistortion.h"

enum
{
    IFX_OFF,
    IFX_DIST,
    IFX_COUNT
};

class IO_AudioEffect : public AudioStream
{
protected:
    audio_block_t *inputQueueArray[1];

    AudioEffectDistortion dist;

public:
    byte currentEffect = IFX_OFF;

    IO_AudioEffect(void) : AudioStream(1, inputQueueArray) {}

    void update()
    {
        if (currentEffect == IFX_OFF)
        {
            audio_block_t *block;
            block = AudioStream::receiveReadOnly();
            if (!block)
                return;
            AudioStream::transmit(block);
            AudioStream::release(block);
            return;
        }
    }
};

#endif
