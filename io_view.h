#ifndef IO_VIEW_H_
#define IO_VIEW_H_

#include <Arduino.h>

#include "instrument/synth/io_audio_synth.h"
#include "io_instrument.h"
#include "io_util.h"

#define MAIN_VIEW_COUNT (SYNTH_COUNT)

byte currentView = 0;

void setCurrentView(int8_t direction) {
    currentView = mod(currentView + direction, MAIN_VIEW_COUNT);
}

IO_AudioSynth* getSynth() { return &synth[currentView]; }

#endif
