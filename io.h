#ifndef IO_SYNTH_H_
#define IO_SYNTH_H_

#include <Arduino.h>

#include "io_audio.h"
#include "io_display.h"
#include "io_instrument.h"
#include "io_midi.h"

void ioInit() {
    Serial.println("Mini synth init");
    displayInit();

    audioInit();
    midiInit();

    displayUpdate();
}

void ioLoop() {
    midiLoop();
    displayLoop();
}

#endif
