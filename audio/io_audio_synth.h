#ifndef IO_AUDIO_SYNTH_H_
#define IO_AUDIO_SYNTH_H_

#include <Arduino.h>
#include <Audio.h>

#include "../../audio/io_audio_filter.h"
#include "../../audio/io_audio_filter_ladder.h"
#include "../../audio/io_audio_waveform.h"
#include "../../audio/io_audio_env.h"
#include "../../audio/note.h"
#include "../../effect/AudioEffectDistortion.h"
#include "../../io_util.h"

class IO_AudioSynth : public IO_AudioEnv
{
public:
    IO_AudioWaveform wave;
    // AudioEffectDistortion distortion;
    IO_AudioFilter filter;
    IO_AudioFilterLadder filterLadder;

    byte lastNote = 0;

    float level = 127;

    AudioConnection *patchCordWaveToFilter;
    AudioConnection *patchCordFilterToFilterLadder;
    AudioConnection *patchCordFilterLadderToEnvOutput;
    // AudioConnection *patchCordFilterToDistortion;
    // AudioConnection *patchCordDistortionToOutput;

    IO_AudioSynth()
    {
        patchCordWaveToFilter = new AudioConnection(wave, filter);
        patchCordFilterToFilterLadder = new AudioConnection(filter, filterLadder);
        patchCordFilterLadderToEnvOutput = new AudioConnection(filterLadder, *this);

        // patchCordFilterToDistortion = new AudioConnection(filter, distortion);
        // patchCordDistortionToOutput = new AudioConnection(distortion, *this);

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

        IO_AudioEnv::noteOn();
        // filter.env.noteOn();
    }

    void noteOff(byte note)
    {
        if (note == lastNote)
        {
            IO_AudioEnv::noteOff();
            // filter.env.noteOff();
        }
    }
};

#endif
