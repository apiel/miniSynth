#ifndef IO_AUDIO_FILTER_LADDER_H_
#define IO_AUDIO_FILTER_LADDER_H_

#include <Arduino.h>
#include <Audio.h>

class IO_AudioFilterLadder : public AudioFilterLadder
{
public:
    byte cutoffPos = 127;
    byte resonancePos = 0;

    IO_AudioFilterLadder() : AudioFilterLadder()
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
        resonance(resonancePos / 127.0f * 1.1f); // * 1.8f
    }

    void update()
    {
        // should find better way to bypass it!!
        if (cutoffPos == 127)
        {
            audio_block_t *block;
            block = AudioStream::receiveReadOnly();
            if (!block)
                return;
            AudioStream::transmit(block);
            AudioStream::release(block);
            return;
        }
        AudioFilterLadder::update();
    }
};

#endif
