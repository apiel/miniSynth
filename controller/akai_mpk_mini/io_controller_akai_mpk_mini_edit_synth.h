#ifndef IO_CONTROLLER_AKAI_MPK_MINI_EDIT_SYNTH_H_
#define IO_CONTROLLER_AKAI_MPK_MINI_EDIT_SYNTH_H_

#include <Arduino.h>

#include "./io_controller_akai_mpk_mini_base.h"

class IO_ControllerAkaiMPKminiEditSynth : virtual public IO_ControllerAkaiMPKminiBase
{
public:
    void controlChangeHandlerEditSynth(byte channel, byte control, byte value)
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
};

#endif
