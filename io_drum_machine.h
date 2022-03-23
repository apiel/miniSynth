#ifndef IO_DRUM_MACHINE_H_
#define IO_DRUM_MACHINE_H_

#include <Arduino.h>
#include <Audio.h>
#include <SD.h>

#include "Pattern.h"
#include "io_drum_patterns.h"

#define DRUM_FILE_LEN 40

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
    char file[DRUM_TYPE_COUNT][DRUM_FILE_LEN];

    byte currentStep = 0;
    Step lastStep;

public:
    byte nextToPlay = 0;
    bool sdAvailable = true;

    bool mute[DRUM_TYPE_COUNT] = {true, true, true, true};

    Pattern *nextPattern = &drumPatterns[0];
    Pattern *pattern = &drumPatterns[0];

    char lastSetFile[DRUM_FILE_LEN];

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

        setDrum(DRUM_KICK, 0);
        setDrum(DRUM_SNARE, 0);
        setDrum(DRUM_HITHAT, 0);
        setDrum(DRUM_MISC, 0);
    }

    void setNextPattern(byte value)
    {
        nextPattern = &drumPatterns[value % DRUM_PATTERN_COUNT];
    }

    bool setDrum(byte drumType, byte value)
    {
        File root = SD.open(getFolder(drumType));
        if (root)
        {
            byte i = 0;
            while (true)
            {
                File entry = root.openNextFile();
                if (!entry)
                {
                    break;
                }
                if (!entry.isDirectory())
                {
                    // Serial.printf("File %s\n", entry.name());
                    if (i == value)
                    {
                        snprintf(file[drumType], DRUM_FILE_LEN, "%s/%s", getFolder(drumType), entry.name());
                        snprintf(lastSetFile, DRUM_FILE_LEN, entry.name());
                        // Serial.printf("SET File %s\n", file[drumType]);
                        entry.close();
                        root.close();
                        return true;
                    }
                    i++;
                }
                entry.close();
            }
            root.close();
        }
        return false;
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

    const char *getFolder(byte drumType)
    {
        switch (drumType)
        {
        case DRUM_KICK:
            return "raw/kick";

        case DRUM_SNARE:
            return "raw/snare";

        case DRUM_HITHAT:
            return "raw/hithat";

        case DRUM_MISC:
            return "raw/misc";
        }
        return "";
    }
};

#endif
