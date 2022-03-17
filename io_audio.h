#ifndef IO_AUDIO_H_
#define IO_AUDIO_H_

#include <Arduino.h>
#include <Audio.h>

#include "io_instruments.h"

AudioOutputMQS audioOut;
AudioMixer4 mixerOutput;

AudioConnection patchCordMixerSynth(mixerOutput, audioOut);
AudioConnection patchCordSynth0(*getInstrument(SYNTH_0), 0, mixerOutput, 0);
AudioConnection patchCordSynth1(*getInstrument(SYNTH_1), 0, mixerOutput, 1);
AudioConnection patchCordSynth2(*getInstrument(SYNTH_2), 0, mixerOutput, 2);
AudioConnection patchCordSynth3(*getInstrument(SYNTH_3), 0, mixerOutput, 3);

void audioInit()
{
    AudioMemory(25);

    // mixerOutput.gain(0, 1.0f);
    // mixerOutput.gain(1, 1.0f);
    // mixerOutput.gain(2, 1.0f);
    // mixerOutput.gain(3, 1.0f);

    for (byte pos = 0; pos < INSTRUMENT_COUNT; pos++)
    {
        getInstrument(pos)->init(pos, instruments, &currentInstrument);
    }
}

#endif
