#ifndef IO_AUDIO_FILTER_H_
#define IO_AUDIO_FILTER_H_

#include <Arduino.h>
#include <Audio.h>

class IO_AudioFilter : public AudioFilterStateVariable
{
public:
    byte cutoffPos = 127;
    byte resonancePos = 0;

    IO_AudioFilter() : AudioFilterStateVariable()
    {
        setCutoff(cutoffPos);
        setResonance(resonancePos);
    }

    void setCutoff(byte value)
    {
        cutoffPos = value;
        float freqVal = value / 127.0f;
        frequency(freqVal * freqVal * 4000.0f);
    }

    void setResonance(byte value)
    {
        resonancePos = value;
        resonance((resonancePos / 127.0f * 4.3) + 0.7);
    }
};

#endif
