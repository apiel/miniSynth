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
    IFX_REVERB,
    IFX_RECTIFIER,
    IFX_BITCRUSHER,
    IFX_COUNT
};

class IO_AudioEffect
{
protected:
    AudioDumb dumb;
    AudioEffectDistortion dist;
    AudioEffectFreeverb reverb;
    AudioEffectRectifier rectifier;
    AudioEffectBitcrusher bitcrusher;

public:
    AudioDumb input;
    AudioDumb output;

    AudioConnection patches[IFX_COUNT][2] = {
        {AudioConnection(input, dumb), AudioConnection(dumb, output)},            // OFF
        {AudioConnection(input, dist), AudioConnection(dist, output)},            // IFX_DIST
        {AudioConnection(input, reverb), AudioConnection(reverb, output)},        // IFX_REVERB
        {AudioConnection(input, rectifier), AudioConnection(rectifier, output)},  // IFX_RECTIFIER
        {AudioConnection(input, bitcrusher), AudioConnection(bitcrusher, output)} // IFX_BITCRUSHER
    };

    byte currentEffect = IFX_OFF;
    byte edit1Value = 0;
    byte edit2Value = 0;

    IO_AudioEffect(void)
    {
        setEffect(currentEffect);
    }

    void edit1(byte value)
    {
        edit1Value = value;

        float pctVal = value / 127.0f;
        switch (currentEffect)
        {
        case IFX_DIST:
            // 1000.0f I am not sure why, maybe it can be more?
            dist.distortion(pctVal * pctVal * 1000.0f);
            break;
        case IFX_REVERB:
            reverb.roomsize(pctVal);
            break;
        case IFX_BITCRUSHER:
            bitcrusher.bits((byte)(pctVal * 16));
            break;
        default:
            break;
        }
    }

    void edit2(byte value)
    {
        edit2Value = value;

        float pctVal = value / 127.0f;
        switch (currentEffect)
        {
        case IFX_DIST:
            dist.setRange((pctVal * pctVal * 999.0f) + 1);
            break;
        case IFX_REVERB:
            reverb.damping(pctVal);
            break;
        case IFX_BITCRUSHER:
            bitcrusher.sampleRate(pctVal * 44100);
            break;
        default:
            break;
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
        case IFX_REVERB:
            return "Reverb";
        case IFX_RECTIFIER:
            return "Rectifier";
        case IFX_BITCRUSHER:
            return "Bitcrusher";
        default:
            return "unknown yet";
        }
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
        edit1(edit1Value);
        edit2(edit2Value);
    }
};

#endif
