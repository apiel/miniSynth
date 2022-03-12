#ifndef AudioFilter_h_
#define AudioFilter_h_

#include <Arduino.h>
#include <Audio.h>

class AudioFilter : public AudioFilterStateVariable
{
public:
    byte cutoffPos = 127;
    byte resonancePos = 0;

    AudioFilter() : AudioFilterStateVariable()
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
