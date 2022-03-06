#ifndef IO_INSTRUMENT_H_
#define IO_INSTRUMENT_H_

#include <Arduino.h>

#include "instrument/synth/io_audio_synth.h"

enum { SYNTH_0, SYNTH_COUNT };

IO_AudioSynth synth[SYNTH_COUNT];

#endif
