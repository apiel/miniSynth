#ifndef IO_INSTRUMENTS_H_
#define IO_INSTRUMENTS_H_

#include <Arduino.h>

#include "instrument/synth/io_audio_synth.h"
#include "instrument/io_audio_base.h"
#include "io_instrument_list.h"

IO_AudioSynth instruments[INSTRUMENT_COUNT];
IO_AudioBase<IO_AudioSynthCore, IO_AudioSynthCoreUI> *instrumentsPtr[INSTRUMENT_COUNT] = {&instruments[SYNTH_0], &instruments[SYNTH_1], &instruments[SYNTH_2], &instruments[SYNTH_3]};
byte currentInstrument = SYNTH_0;

// void setCurrent(byte value)
// {
//     current = value % INSTRUMENT_COUNT;
// }

IO_AudioSynth *getInstrument() { return &instruments[currentInstrument]; }
IO_AudioSynth *getInstrument(byte pos) { return &instruments[pos % INSTRUMENT_COUNT]; }

#endif
