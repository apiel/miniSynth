#ifndef IO_AUDIO_H_
#define IO_AUDIO_H_

#include <Arduino.h>
#include <Audio.h>

#include "io_instruments.h"

AudioOutputMQS audioOut;
AudioMixer4 mixerOutput;

AudioConnection patchCordMixerSynth(mixerOutput, audioOut);
AudioConnection patchCordSynth0(*getInstrument(SYNTH_0), mixerOutput);
AudioConnection patchCordSynth1(*getInstrument(SYNTH_1), mixerOutput);
AudioConnection patchCordSynth2(*getInstrument(SYNTH_2), mixerOutput);
AudioConnection patchCordSynth3(*getInstrument(SYNTH_3), mixerOutput);

void audioInit()
{
    AudioMemory(25);

    mixerOutput.gain(0, 1.0f);
    mixerOutput.gain(1, 1.0f);
    mixerOutput.gain(2, 1.0f);
    mixerOutput.gain(3, 1.0f);

    for (byte pos = 0; pos < INSTRUMENT_COUNT; pos++)
    {
        getInstrument(pos)->init(pos, instrumentsPtr, &currentInstrument);
    }
}

#endif
