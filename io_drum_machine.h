#ifndef IO_DRUM_MACHINE_H_
#define IO_DRUM_MACHINE_H_

#include <Arduino.h>
#include <Audio.h>
#include <SD.h>

#include "Pattern.h"
#include "io_drum_patterns.h"

enum
{
    DRUM_KICK,
    DRUM_SNARE,
    DRUM_HITHAT,
    DRUM_MISC,
    DRUM_TYPE_COUNT
};

class IO_DrumMachine
{
protected:
    char file[DRUM_TYPE_COUNT][40];

    byte currentStep = 0;
    Step lastStep;

public:
    byte nextToPlay = 0;
    bool sdAvailable = true;

    bool mute[DRUM_TYPE_COUNT] = {true, true, true, true};

    Pattern *nextPattern = &drumPatterns[0];
    Pattern *pattern = &drumPatterns[0];

    // should we use AudioPlayMemory to be able to play multiple sound at once
    // maybe create IO_DrumMachineMem
    AudioPlaySdRaw sample;

    IO_DrumMachine()
    {
    }

    void init()
    {
        if (!(SD.begin(BUILTIN_SDCARD)))
        {
            Serial.println("Unable to access the SD card");
            sdAvailable = false;
        }
        else
        {
            Serial.println("SD card ready");
        }

        sprintf(file[DRUM_KICK], "raw/kick/kick003.raw");
        sprintf(file[DRUM_SNARE], "raw/snare/snare1.raw");
        sprintf(file[DRUM_HITHAT], "raw/hithat/hithat1.raw");
        sprintf(file[DRUM_MISC], "raw/misc/guitar2.raw");
    }

    void setNextPattern(byte value)
    {
        nextPattern = &drumPatterns[value % DRUM_PATTERN_COUNT];
    }

    void setDrum(byte drumType, byte value)
    {
        Serial.printf("set drum %d -> %d\n", drumType, value);
    }

    void next()
    {
        Step *step = &pattern->steps[currentStep];
        if (step->note > 0)
        {
            // note will define which drum type kick, hh, snar, ...
            if (step->note == KICK && !mute[DRUM_KICK])
            {
                sample.play(file[DRUM_KICK]);
                Serial.printf("Play kick %s\n", file[DRUM_KICK]);
            }
            else if (step->note == SNARE && !mute[DRUM_SNARE])
            {
                sample.play(file[DRUM_SNARE]);
            }
            else if (step->note == HITHAT && !mute[DRUM_HITHAT])
            {
                sample.play(file[DRUM_HITHAT]);
            }
            else if (step->note == MISC && !mute[DRUM_MISC])
            {
                sample.play(file[DRUM_MISC]);
            }
        }
        currentStep = (currentStep + 1) % pattern->stepCount;

        if (currentStep == 0)
        {
            pattern = nextPattern;
        }
    }

    void toggleMute(byte drumType)
    {
        mute[drumType % DRUM_TYPE_COUNT] = !mute[drumType % DRUM_TYPE_COUNT];
    }

    const char *getDrumName(byte drumType)
    {
        switch (drumType)
        {
        case DRUM_KICK:
            return "Kick";

        case DRUM_SNARE:
            return "Snare";

        case DRUM_HITHAT:
            return "Hithat";

        case DRUM_MISC:
            return "Misc";
        }
        return "Unknown";
    }
};

#endif
