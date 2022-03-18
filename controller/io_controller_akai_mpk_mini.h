#ifndef IO_AUDIO_BASE_H_
#define IO_AUDIO_BASE_H_

#include <Arduino.h>
#include <Audio.h>

#include "../audio/io_audio_synth.h"
#include "../io_audio_loop.h"
#include "../io_instrument_list.h"
#include "../io_display.h"

class IO_ControllerAkaiMPKmini
{
protected:
    IO_AudioLoop *loopPadPressed = NULL;
    bool loopPadPressedDidAction = false;
    bool mode = false;

    IO_AudioLoop **loops;
    IO_AudioSynth **synths;

    IO_AudioLoop *loop;
    IO_AudioSynth *synth;

    IO_Display *display;

public:
    byte currentPattern = 0;
    Pattern *pattern;

    // for the moment will always be arpMode
    bool arpMode = true;

    IO_ControllerAkaiMPKmini(IO_Display *_display, IO_AudioLoop **_loops, IO_AudioSynth **_synths)
    {
        display = _display;
        loops = _loops;
        synths = _synths;
        loop = getLoop(0);
        synth = getSynth(0);
    }

    IO_AudioSynth *getSynth(byte pos) { return synths[pos % SYNTH_COUNT]; }
    IO_AudioLoop *getLoop(byte pos) { return loops[pos % SYNTH_COUNT]; }

    void setCurrentSynth(byte value)
    {
        byte pos = value % SYNTH_COUNT;
        loop = getLoop(pos);
        synth = getSynth(pos);
    }

    void noteOnHandler(byte channel, byte note, byte velocity)
    {
        if (channel == 10)
        {
            if (note == 36)
            {
                loopPadPressed = getLoop(SYNTH_0);
            }
            else if (note == 37)
            {
                loopPadPressed = getLoop(SYNTH_1);
            }
            else if (note == 38)
            {
                loopPadPressed = getLoop(SYNTH_2);
            }
            else if (note == 39)
            {
                loopPadPressed = getLoop(SYNTH_3);
            }
            return;
        }

        if (arpMode)
        {
            loop->noteOn(note);
        }
        else
        {
            synth->noteOn(note, velocity);
        }
    }

    void loopPadPressedAction(byte id)
    {
        if (loop->id != id)
        {
            setCurrentSynth(id);
        }
        else if (!loopPadPressedDidAction)
        {
            loop->toggleMode();
        }
        loopPadPressed = NULL;
        loopPadPressedDidAction = false;
    }

    void noteOffHandler(byte channel, byte note, byte velocity)
    {
        if (channel == 10)
        {
            if (note == 36)
            {
                loopPadPressedAction(0);
            }
            else if (note == 37)
            {
                loopPadPressedAction(1);
            }
            else if (note == 38)
            {
                loopPadPressedAction(2);
            }
            else if (note == 39)
            {
                loopPadPressedAction(3);
            }
            else if (note == 40)
            {
                loop->setCurrentPatternSelector(0);
                display->displayValue("Pattern selector 0", loop->patternSelector[0]);
            }
            else if (note == 41)
            {
                loop->setCurrentPatternSelector(1);
                display->displayValue("Pattern selector 1", loop->patternSelector[1]);
            }
            else if (note == 42)
            {
                loop->setCurrentPatternSelector(2);
                display->displayValue("Pattern selector 2", loop->patternSelector[2]);
            }
            else if (note == 43)
            {
                loop->setCurrentPatternSelector(3);
                display->displayValue("Pattern selector 3", loop->patternSelector[3]);
            }
            return;
        }

        if (arpMode)
        {
            loop->noteOff(note);
        }
        else
        {
            synth->noteOff(note);
        }
    }

    void controlChangeHandler(byte channel, byte control, int8_t direction, byte value)
    {
        if (loopPadPressed)
        {
            if (control == 17)
            {
                loopPadPressed->setPatternSelector(0, value);
                display->displayValue("Pattern selector 0", loopPadPressed->patternSelector[0]);
                loopPadPressedDidAction = true;
            }
            else if (control == 18)
            {
                loopPadPressed->setPatternSelector(1, value);
                display->displayValue("Pattern selector 1", loopPadPressed->patternSelector[1]);
                loopPadPressedDidAction = true;
            }
            else if (control == 19)
            {
                loopPadPressed->setPatternSelector(2, value);
                display->displayValue("Pattern selector 2", loopPadPressed->patternSelector[2]);
                loopPadPressedDidAction = true;
            }
            else if (control == 20)
            {
                loopPadPressed->setPatternSelector(3, value);
                display->displayValue("Pattern selector 3", loopPadPressed->patternSelector[3]);
                loopPadPressedDidAction = true;
            }
            return;
        }

        if (control == 64) // when pressin sustain button
        {
            mode = value == 127;
            // mode = value != 127;
        }
        else if (mode)
        {
            // bottom row
            if (control == 13)
            {
                synth->wave.setWaveform(value);
            }
            else if (control == 14)
            {
                synth->wave.edit(value);
                display->displayValue("Wave edit", value);
            }
            else if (control == 15)
            {
            }
            else if (control == 16)
            {
            }
            // top row
            else if (control == 17)
            {
            }
            else if (control == 18)
            {
            }
            else if (control == 19)
            {
            }
            else if (control == 20)
            {
                synth->setLevel(value);
                display->displayValue("Level", value);
            }
        }
        else
        {
            // bottom row
            if (control == 13)
            {
                synth->filter.setResonance(value);
                display->displayValue("Resonnance", value);
            }
            else if (control == 14)
            {
                synth->filterLadder.setResonance(value);
                display->displayValue("Ladder Resonnance", value);
            }
            else if (control == 15)
            {
            }
            else if (control == 16)
            {
                synth->setRelease(value);
                display->displayMs("Env. Release", synth->releaseMs);
            }
            // top row
            else if (control == 17)
            {
                synth->filter.setCutoff(value);
                display->displayValue("Cutoff", value);
            }
            else if (control == 18)
            {
                synth->filterLadder.setCutoff(value);
                display->displayValue("Ladder Cutoff", value);
            }
            else if (control == 19)
            {
            }
            else if (control == 20)
            {
                synth->setAttack(value);
                display->displayMs("Env. Attack", synth->attackMs);
            }
        }
    }
};

#endif
