#ifndef IO_AUDIO_H_
#define IO_AUDIO_H_

#include <Arduino.h>
#include <Audio.h>

#include "io_instrument.h"

AudioOutputMQS audioOut;
AudioMixer4 mixerSynth;

AudioConnection patchCordMixerSynth(mixerSynth, audioOut);
AudioConnection patchCordSynth0(synth[SYNTH_0], mixerSynth);
AudioConnection patchCordSynth1(synth[SYNTH_1], mixerSynth);
AudioConnection patchCordSynth2(synth[SYNTH_2], mixerSynth);
AudioConnection patchCordSynth3(synth[SYNTH_3], mixerSynth);

void audioInit()
{
    AudioMemory(25);
    for (byte pos = 0; pos < SYNTH_COUNT; pos++)
    {
        synth[pos].init(pos);
    }
}

#endif
