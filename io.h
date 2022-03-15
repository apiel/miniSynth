#ifndef IO_SYNTH_H_
#define IO_SYNTH_H_

#include <Arduino.h>

#include "io_audio.h"
#include "io_display.h"
#include "io_instruments.h"
#include "io_midi.h"
#include "io_tempo.h"

void ioInit()
{
    Serial.println("Mini synth init");
    displayInit();

    audioInit();
    midiInit();

    displayUpdate();
    tempoInit();
}

void ioLoop()
{
    midiLoop();
    displayLoop();
    tempoLoop();
}

#endif
