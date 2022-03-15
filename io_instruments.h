#ifndef IO_INSTRUMENTS_H_
#define IO_INSTRUMENTS_H_

#include <Arduino.h>

#include "instrument/synth/io_audio_synth.h"
#include "instrument/io_audio_base.h"
#include "io_instrument_list.h"

IO_AudioSynth _instruments[INSTRUMENT_COUNT];
IO_AudioBase<IO_AudioSynthCore, IO_AudioSynthCoreUI> *instruments[INSTRUMENT_COUNT] = {
    &_instruments[SYNTH_0],
    &_instruments[SYNTH_1],
    &_instruments[SYNTH_2],
    &_instruments[SYNTH_3]};
byte currentInstrument = SYNTH_0;

// see how to make this generic
IO_AudioSynth *getInstrument() { return &_instruments[currentInstrument]; }
IO_AudioSynth *getInstrument(byte pos) { return &_instruments[pos % INSTRUMENT_COUNT]; }

#endif
