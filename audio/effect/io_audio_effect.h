#ifndef IO_AUDIO_EFFECT_H_
#define IO_AUDIO_EFFECT_H_

#include <Arduino.h>
#include <Audio.h>

#include "AudioEffectDistortion.h"
#include "../audio_dumb.h"

enum
{
    IFX_OFF,
    IFX_DIST,
    IFX_COUNT
};

class IO_AudioEffect
{
protected:
    AudioDumb dumb;
    AudioEffectDistortion dist;

public:
    byte currentEffect = IFX_DIST;

    AudioStream *stream;

    IO_AudioEffect(void)
    {
        setEffect(IFX_OFF);
    }

    void setEffect(byte value)
    {
        currentEffect = value % IFX_COUNT;
        // should we somehow release some block here??
        if (currentEffect == IFX_OFF)
        {
            stream = &dumb;
        }
        else if (currentEffect == IFX_DIST)
        {
            stream = &dist;
        }
    }

    const char *getName()
    {

        switch (currentEffect)
        {
        case IFX_OFF:
            return "Off";
        case IFX_DIST:
            return "Distortion";
        default:
            return "unknown yet";
        }
    }
};

#endif
