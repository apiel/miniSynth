#ifndef IO_AUDIO_SYNTH_CORE_H_
#define IO_AUDIO_SYNTH_CORE_H_

#include <Arduino.h>
#include <Audio.h>

#include "../../audio/AudioFilter.h"
#include "../../audio/audio_dumb.h"
#include "../../audio/note.h"
#include "../../effect/AudioEffectDistortion.h"
#include "../../io_util.h"
#include "../../wavetable/guitar01.h"

class IO_AudioSynthCore : public AudioDumb
{
protected:
public:
    AudioSynthWaveform wave;
    AudioEffectEnvelope env;
    AudioEffectDistortion distortion;
    AudioFilter filter;

    byte lastNote = 0;

    float attackMs = 10.0;
    float releaseMs = 50.0;

    byte currentWave = WAVEFORM_SINE;
    float amplitude = 1.0;

    AudioConnection *patchCordWaveToEnv;
    AudioConnection *patchCordEnvToFilter;
    AudioConnection *patchCordFilterToDistortion;
    AudioConnection *patchCordDistortionToOutput;

    Guitar01 table;

    IO_AudioSynthCore()
    {
        patchCordWaveToEnv = new AudioConnection(wave, env);
        patchCordEnvToFilter = new AudioConnection(env, filter.input);
        patchCordFilterToDistortion = new AudioConnection(filter, distortion);
        patchCordDistortionToOutput = new AudioConnection(distortion, *this);

        env.hold(0);
        env.attack(attackMs); // ms
        env.decay(0);
        env.sustain(1.0); // level
        env.release(releaseMs); // ms

        wave.amplitude(amplitude);
        wave.arbitraryWaveform(table.table, 172.0);
        wave.begin(currentWave);

        distortion.distortion(0.5);
    }

    void setAttack(byte value)
    {
        attackMs = 10.0f * value;
        env.attack(attackMs);
    }

    void setLevel(byte value)
    {
        amplitude = ((float)value) / 127.0f;
        wave.amplitude(amplitude);
    }

    void setRelease(byte value)
    {
        releaseMs = 20.0f * value;
        env.release(releaseMs);
    }

    void noteOn(byte note, byte velocity)
    {
        lastNote = note;

        // float _amp = amplitude * velocity / 127;

        // this is not ok :p
        // setAmplitude(_amp);
        wave.frequency(NOTE_FREQ[note]);

        env.noteOn();
        // filter.env.noteOn();
    }

    void noteOff(byte note)
    {
        if (note == lastNote)
        {
            env.noteOff();
            // filter.env.noteOff();
        }
    }
};

#endif
