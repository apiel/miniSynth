#ifndef IO_AUDIO_LOOP_H_
#define IO_AUDIO_LOOP_H_

#include <Arduino.h>

#include "Pattern.h"
#include "io_arp_patterns.h"
#include "audio/io_audio_synth.h""

#define REF_NOTE _C4
#define PATTERN_SELECTOR_COUNT 4

/**
 *
 * Use key combination to change the notes of the loop
 * so there is very basic pattern where not can be modified on the fly
 *
 * How to handle params lock? This should be directly in the synth part or?
 * Maybe could use a note to reference note
 *
 */

class IO_AudioLoop
{
private:
    IO_AudioSynth *env;

    byte currentStep = 0;
    Step lastStep;

public:
    byte id = 0;
    bool active = true;
    bool modeSingleLoop = true;
    byte nextToPlay = 0;
    byte play = 0;
    byte previousLoopNote = 0;

    byte patternSelector[PATTERN_SELECTOR_COUNT] = {2, 1, 0, 0};
    byte currentPatternSelector = 0;
    byte nextPattern = 0;
    Pattern *pattern = &patterns[nextPattern];

    IO_AudioLoop(IO_AudioSynth *_env, byte _id) : IO_AudioLoop(_env)
    {
        id = id;
    }

    IO_AudioLoop(IO_AudioSynth *_env)
    {
        env = _env;
        setCurrentPatternSelector(currentPatternSelector);
    }

    void setPatternSelector(byte pos, byte value)
    {
        patternSelector[pos % PATTERN_SELECTOR_COUNT] = value;
    }

    void setCurrentPatternSelector(byte value)
    {
        currentPatternSelector = value % PATTERN_SELECTOR_COUNT;
        nextPattern = patternSelector[currentPatternSelector] % ARP_PATTERN_COUNT;
    }

    void next()
    {
        if (!lastStep.slide)
        {
            env->noteOff(lastStep.note);
            // to avoid repeating this again, let set slide to true
            lastStep.slide = true;
        }

        if (play)
        {
            Step *step = &pattern->steps[currentStep];
            if (step->note > 0)
            {
                lastStep.set(step);
                // add note difference to note
                lastStep.note += (int)play - (int)REF_NOTE;
                env->noteOn(lastStep.note, lastStep.velocity);
                // could have mode with no substain here
                // and directly noteOff
                // for drum?
                // but should this be part of the envelop and not from the sequencer
            }
        }
        currentStep = (currentStep + 1) % pattern->stepCount;

        if (currentStep == 0)
        {
            pattern = &patterns[nextPattern];
            play = nextToPlay ? nextToPlay : 0;
        }
    }

    void noteOn(byte note)
    {
        if (active)
        {
            nextToPlay = note;
        }
    }

    void noteOff(byte note)
    {
        if (modeSingleLoop && note == nextToPlay)
        {
            nextToPlay = 0;
        }
    }

    void toggle() { activate(!active); }
    void activate(bool value = true) { active = value; }

    void toggleMode() { activateSingleLoopMode(!modeSingleLoop); }
    void activateSingleLoopMode(bool value = true)
    {
        modeSingleLoop = value;
        if (modeSingleLoop)
        {
            previousLoopNote = nextToPlay;
            nextToPlay = 0;
        }
        else
        {
            nextToPlay = previousLoopNote;
        }
    }
};

#endif
