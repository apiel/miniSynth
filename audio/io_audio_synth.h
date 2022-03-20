#ifndef IO_AUDIO_SYNTH_H_
#define IO_AUDIO_SYNTH_H_

#include <Arduino.h>
#include <Audio.h>

#include "io_audio_filter.h"
#include "io_audio_filter_ladder.h"
#include "io_audio_waveform.h"
#include "io_audio_env.h"
#include "effect/io_audio_effect.h"
#include "note.h"

class IO_AudioSynth : public IO_AudioEnv
{
public:
    IO_AudioWaveform wave;
    IO_AudioEffect effect;
    IO_AudioFilter filter;
    IO_AudioFilterLadder filterLadder;

    byte lastNote = 0;

    float level = 127;

    AudioConnection *patchCordWaveToFilter;
    AudioConnection *patchCordFilterToFilterLadder;
    AudioConnection *patchCordFilterLadderToEffect;
    AudioConnection *patchCordEffectToEnvOutput;
    // AudioConnection *patchCordFilterToDistortion;
    // AudioConnection *patchCordDistortionToOutput;

    IO_AudioSynth()
    {
        patchCordWaveToFilter = new AudioConnection(wave, filter);
        patchCordFilterToFilterLadder = new AudioConnection(filter, filterLadder);
        patchCordFilterLadderToEffect = new AudioConnection(filterLadder, effect);
        patchCordEffectToEnvOutput = new AudioConnection(effect, *this);

        setLevel(level);
        wave.begin();

        // distortion.distortion(0.0);
    }

    void setLevel(byte value)
    {
        level = value;
        // could also use envelop substain level?
        // seem the amplitude to be more clean
        wave.amplitude(((float)level) / 127.0f);
    }

    void noteOn(byte note, byte velocity)
    {
        lastNote = note;

        // float _amp = amplitude * velocity / 127;

        // this is not ok :p
        // setAmplitude(_amp);
        wave.frequency(NOTE_FREQ[note]);

        IO_AudioEnv::noteOn(note, velocity);
        // filter.env.noteOn();
    }

    void noteOff(byte note)
    {
        if (note == lastNote)
        {
            IO_AudioEnv::noteOff(note);
            // filter.env.noteOff();
        }
    }
};

#endif
