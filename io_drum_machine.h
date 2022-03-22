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
    char filename[40];

    byte currentStep = 0;
    Step lastStep;

public:
    bool active = true;
    byte nextToPlay = 0;
    // byte play = 0;
    byte play = 1;
    bool sdAvailable = true;

    // byte patternSelector[PATTERN_SELECTOR_COUNT] = {2, 1, 0, 0};
    byte currentPatternSelector = 0;
    byte nextPattern = 0;
    Pattern *pattern = &drumPatterns[nextPattern];

    // should we use AudioPlayMemory to be able to play multiple sound at once
    // maybe create IO_DrumMachineMem
    AudioPlaySdRaw sample;

    IO_DrumMachine()
    {
        setCurrentPatternSelector(currentPatternSelector);

        if (!(SD.begin(BUILTIN_SDCARD)))
        {
            Serial.println("Unable to access the SD card");
            sdAvailable = false;
        } else {
            Serial.println("SD card ready");
        }
    }

    void setPatternSelector(byte pos, byte value)
    {
        // patternSelector[pos % PATTERN_SELECTOR_COUNT] = value;
    }

    void setCurrentPatternSelector(byte value)
    {
        // currentPatternSelector = value % PATTERN_SELECTOR_COUNT;
        // nextPattern = patternSelector[currentPatternSelector] % ARP_PATTERN_COUNT;
    }

    void next()
    {
        if (play)
        {
            Step *step = &pattern->steps[currentStep];
            if (step->note > 0)
            {
                // note will define which drum type kick, hh, snar, ...
                if (step->note == KICK)
                {
                    sprintf(filename, "raw/kick/kick003.raw");
                }
                else if (step->note == SNARE)
                {
                    sprintf(filename, "raw/snare/snare1.raw");
                }
                else
                {
                    sprintf(filename, "raw/hithat/hithat1.raw");
                }

                sample.play(filename);
            }
        }
        currentStep = (currentStep + 1) % pattern->stepCount;

        if (currentStep == 0)
        {
            // pattern = &patterns[nextPattern];
            // play = nextToPlay ? nextToPlay : 0;
        }
    }

    void toggle() { activate(!active); }
    void activate(bool value = true) { active = value; }

    // instead of activate,
    // might just mute kick, hh, snare...
};

#endif
