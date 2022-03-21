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

// to solve https://forum.pjrc.com/threads/58566-Changing-effects-at-run-time
// or https://forum.pjrc.com/threads/64446-Dynamic-Audio-Connection-Bug-A-new-one

class IO_AudioEffect
{
protected:
    AudioDumb dumb;
    AudioEffectDistortion dist;

public:
    AudioDumb input;
    AudioDumb output;

    AudioConnection patches[IFX_COUNT][2] = {
        {AudioConnection(input, dumb), AudioConnection(dumb, output)}, // OFF
        {AudioConnection(input, dist), AudioConnection(dist, output)}  // IFX_DIST
    };

    byte currentEffect = IFX_DIST;
    // byte currentEffect = IFX_OFF;

    IO_AudioEffect(void)
    {
        setEffect(currentEffect);
    }

    void setEffect(byte value)
    {
        currentEffect = value % IFX_COUNT;
        // should we somehow release some block here??
        for (byte i = 0; i < IFX_COUNT; i++)
        {
            if (i == currentEffect)
            {
                patches[i][0].connect();
                patches[i][1].connect();
            }
            else
            {
                patches[i][0].disconnect();
                patches[i][1].disconnect();
            }
        }
    }

    void edit1(byte value)
    {
        float pctVal = value / 127.0f;
        if (currentEffect == IFX_DIST)
        {
            // 1000.0f i am not sure why, maybe it can be more?
            dist.distortion(pctVal * pctVal * 1000.0f);
            // Serial.printf("Edit distor %.6f\n", pctVal * pctVal * 1000.0f);
        }
    }

    void edit2(byte value)
    {
        float pctVal = value / 127.0f;
        if (currentEffect == IFX_DIST)
        {
            dist.setRange((pctVal * pctVal * 999.0f) + 1);
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
