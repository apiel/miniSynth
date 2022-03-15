#ifndef IO_INSTRUMENT_H_
#define IO_INSTRUMENT_H_

#include <Arduino.h>

#include "instrument/synth/io_audio_synth.h"

enum
{
    SYNTH_0,
    SYNTH_1,
    SYNTH_2,
    SYNTH_3,
    SYNTH_COUNT
};

IO_AudioSynth synth[SYNTH_COUNT];

byte currentSynth = 0;

void setCurrentSynth(byte value)
{
    currentSynth = value % SYNTH_COUNT;
}

byte getCurrentSynth()
{
    return currentSynth;
}

IO_AudioSynth *getSynth() { return &synth[currentSynth]; }
IO_AudioSynth *getSynth(byte pos) { return &synth[pos % SYNTH_COUNT]; }

#endif
