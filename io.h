#ifndef IO_H_
#define IO_H_

#include <Arduino.h>
#include <Audio.h>

#include "io_display.h"
#include "io_midi.h"
#include "io_tempo.h"
#include "io_core.h"

void ioInit()
{
    Serial.println("Mini synth init");
    display.init();

    AudioMemory(25);

    midiInit();

    display.update();
    tempoInit();
}

void ioLoop()
{
    midiLoop();
    display.loop();
    tempoLoop(loopsPtr);
}

#endif
