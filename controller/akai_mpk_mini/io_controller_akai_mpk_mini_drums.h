#ifndef IO_CONTROLLER_AKAI_MPK_DRUMS_H_
#define IO_CONTROLLER_AKAI_MPK_DRUMS_H_

#include <Arduino.h>

#include "./io_controller_akai_mpk_mini_base.h"

class IO_ControllerAkaiMPKminiDrums : virtual public IO_ControllerAkaiMPKminiBase
{
public:
    void noteOffDrum(byte channel, byte note, byte velocity)
    {
        if (channel == PAD_CHANNEL)
        {
            if (note == PAD_1)
            {
                drums->toggleMute(KICK);
                display->displayString("Kick", drums->kickMute ? "Mute" : "ON");
            }
            else if (note == PAD_2)
            {
                drums->toggleMute(HITHAT);
                display->displayString("Hithat", drums->hithatMute ? "Mute" : "ON");
            }
            else if (note == PAD_3)
            {
                drums->toggleMute(SNARE);
                display->displayString("Snare", drums->snareMute ? "Mute" : "ON");
            }
            else if (note == PAD_4)
            {
                drums->toggleMute(MISC);
                display->displayString("Misc", drums->miscMute ? "Mute" : "ON");
            }
            return;
        }
        else
        {
            drums->setNextPattern(note - 48);
            display->displayValue("Drum pattern", note - 48);
        }
    }
};

#endif
