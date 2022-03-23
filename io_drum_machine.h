#ifndef IO_DRUM_MACHINE_H_
#define IO_DRUM_MACHINE_H_

#include <Arduino.h>
#include <Audio.h>
#include <SD.h>

#include "Pattern.h"
#include "io_drum_patterns.h"

class IO_DrumMachine
{
protected:
    char kickFile[40];
    char hithatFile[40];
    char snareFile[40];
    char miscFile[40];

    byte currentStep = 0;
    Step lastStep;

public:
    byte nextToPlay = 0;
    bool sdAvailable = true;

    bool kickMute = true;
    bool hithatMute = true;
    bool snareMute = true;
    bool miscMute = true;

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

        sprintf(kickFile, "raw/kick/kick003.raw");
        sprintf(snareFile, "raw/snare/snare1.raw");
        sprintf(hithatFile, "raw/hithat/hithat1.raw");
        sprintf(miscFile, "raw/misc/guitar2.raw");
    }

    void setNextPattern(byte value)
    {
        nextPattern = &drumPatterns[value % DRUM_PATTERN_COUNT];
    }

    void next()
    {
        Step *step = &pattern->steps[currentStep];
        if (step->note > 0)
        {
            // note will define which drum type kick, hh, snar, ...
            if (step->note == KICK && !kickMute)
            {
                sample.play(kickFile);
            }
            else if (step->note == SNARE && !snareMute)
            {
                sample.play(snareFile);
            }
            else if (step->note == HITHAT && !hithatMute)
            {
                sample.play(hithatFile);
            }
            else if (step->note == MISC && !miscMute)
            {
                sample.play(miscFile);
            }
        }
        currentStep = (currentStep + 1) % pattern->stepCount;

        if (currentStep == 0)
        {
            pattern = nextPattern;
        }
    }

    void toggleMute(byte note)
    {
        switch (note)
        {
        case KICK:
            kickMute = !kickMute;
            break;

        case SNARE:
            snareMute = !snareMute;
            break;

        case HITHAT:
            hithatMute = !hithatMute;
            break;

        case MISC:
            miscMute = !miscMute;
            break;

        default:
            break;
        }
    }
};

#endif
