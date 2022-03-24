#ifndef IO_DRUM_MACHINE_H_
#define IO_DRUM_MACHINE_H_

#include <Arduino.h>
#include <Audio.h>
#include <SD.h>

#include "Pattern.h"
#include "io_drum_patterns.h"

#define DRUM_FILE_LEN 40
#define DRUM_DIR_LEN 25 // could be more but let's stick to the number of key on the keyboard

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
    char file[DRUM_TYPE_COUNT][DRUM_DIR_LEN][DRUM_FILE_LEN];
    char *currentFile[DRUM_TYPE_COUNT] = {NULL, NULL, NULL, NULL};

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
            loadDir(DRUM_KICK);
            loadDir(DRUM_SNARE);
            loadDir(DRUM_HITHAT);
            loadDir(DRUM_MISC);

            setDrum(DRUM_KICK, 0);
            setDrum(DRUM_SNARE, 0);
            setDrum(DRUM_HITHAT, 0);
            setDrum(DRUM_MISC, 0);
        }
    }

    void loadDir(byte drumType)
    {
        File root = SD.open(getFolder(drumType));
        if (root)
        {
            for (byte i = 0; i < DRUM_DIR_LEN; i++)
            {
                File entry = root.openNextFile();
                if (!entry)
                {
                    // still set all the empty places
                    for (; i < DRUM_DIR_LEN; i++)
                    {
                        file[drumType][i][0] = '\0';
                    }
                    break;
                }
                if (!entry.isDirectory())
                {
                    snprintf(file[drumType][i], DRUM_FILE_LEN, "%s/%s", getFolder(drumType), entry.name());
                }
                entry.close();
            }
            root.close();
        }
    }

    void setNextPattern(byte value)
    {
        nextPattern = &drumPatterns[value % DRUM_PATTERN_COUNT];
    }

    char * setDrum(byte drumType, byte value)
    {
        if (file[drumType][value % DRUM_DIR_LEN][0] != '\0')
        {
            currentFile[drumType] = &file[drumType][value % DRUM_DIR_LEN][0];
            return &file[drumType][value % DRUM_DIR_LEN][0] + strlen(getFolder(drumType)) + 1;
        }
        return NULL;
    }

    void next()
    {
        Step *step = &pattern->steps[currentStep];
        if (step->note > 0)
        {
            // note will define which drum type kick, hh, snar, ...
            if (step->note == KICK && !mute[DRUM_KICK] && currentFile[DRUM_KICK])
            {
                sample.play(currentFile[DRUM_KICK]);
            }
            else if (step->note == SNARE && !mute[DRUM_SNARE] && currentFile[DRUM_SNARE])
            {
                sample.play(currentFile[DRUM_SNARE]);
            }
            else if (step->note == HITHAT && !mute[DRUM_HITHAT] && currentFile[DRUM_HITHAT])
            {
                sample.play(currentFile[DRUM_HITHAT]);
            }
            else if (step->note == MISC && !mute[DRUM_MISC] && currentFile[DRUM_MISC])
            {
                sample.play(currentFile[DRUM_MISC]);
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
