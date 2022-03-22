#ifndef IO_TEMPO_H_
#define IO_TEMPO_H_

#include <Arduino.h>
#include <Metro.h>

#include "io_instrument_list.h"

// 4 ♬ ♬ step per beat
#define STEP_PER_BEAT 4
#define BPM_START 73

byte bpm = 80;
unsigned int tempo = 187;
Metro timer = Metro(tempo);

void setTempo(byte value) {
    bpm = value + 73; // let say will not go under 73, there we can reach 200 bpm
    // 4 ♬ ♬ step per beat in 60000ms = 1min
    tempo = 60000 / (bpm * STEP_PER_BEAT);
    timer.interval(tempo);
}

void tempoInit() { setTempo(bpm - BPM_START); }

void tempoLoop(IO_AudioLoop **loops, IO_DrumMachine * drumMachine) {
    if (timer.check() == 1) {
        timer.reset();
        for (byte pos = 0; pos < SYNTH_COUNT; pos++) {
            loops[pos]->next();
        }
        drumMachine->next();
    }
}

#endif
