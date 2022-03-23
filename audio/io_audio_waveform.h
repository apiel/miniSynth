#ifndef IO_AUDIO_WAVEFORM_H_
#define IO_AUDIO_WAVEFORM_H_

#include <Arduino.h>
#include <Audio.h>

#include "../wavetable/guitar01.h"

class IO_AudioWaveform : public AudioSynthWaveform
{
public:
    Guitar01 table;
    byte currentWave = WAVEFORM_SAWTOOTH;
    byte editpos = 64;

    void begin(short t_type)
    {
        AudioSynthWaveform::begin(currentWave);
    }

    void begin()
    {
        arbitraryWaveform(table.table, 172.0);
        begin(currentWave);
    }

    void edit(byte value)
    {
        pulseWidth(value / 127.0f);
        offset((value / 127.0f * 2) - 1);
    }

    void setWaveform(byte value)
    {
        currentWave = (value / 128.0f) * 13;

        AudioNoInterrupts();
        begin(currentWave);
        AudioInterrupts();
    }

    const char *getName()
    {

        switch (currentWave)
        {
        case 0:
            return "Sine";
        case 1:
            return "Sawtooth";
        case 2:
            return "Square";
        case 3:
            return "Triangle";
        case 4:
            return "Arbitrary";
        case 5:
            return "Pulse";
        case 6:
            return "Saw reverse";
        case 7:
            return "Sample hold";
        case 8:
            return "Tri var";
        case 9:
            return "Band sawtooth";
        case 10:
            return "Band saw reverse";
        case 11:
            return "Band sqaure";
        case 12:
            return "Band pulse";
        }
        return "Unknown";
    }
};

#endif
