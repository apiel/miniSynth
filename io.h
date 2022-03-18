#ifndef IO_SYNTH_H_
#define IO_SYNTH_H_

#include <Arduino.h>

#include "io_audio.h"
#include "io_display.h"
#include "io_midi.h"
#include "io_tempo.h"
#include "audio/io_audio_synth.h"
#include "io_audio_loop.h"
#include "io_instrument_list.h"
#include "controller/io_controller_akai_mpk_mini.h"

IO_Display display;
IO_AudioSynth synths[SYNTH_COUNT];
IO_AudioLoop loop0(&synths[SYNTH_0], SYNTH_0);
IO_AudioLoop loop1(&synths[SYNTH_1], SYNTH_1);
IO_AudioLoop loop2(&synths[SYNTH_2], SYNTH_2);
IO_AudioLoop loop3(&synths[SYNTH_3], SYNTH_3);
// IO_AudioLoop **loops = ;

IO_ControllerAkaiMPKmini controller(
    &display,
    {&loop0, &loop1, &loop2, &loop3},
    {&synths[SYNTH_0], &synths[SYNTH_1], &synths[SYNTH_2], &synths[SYNTH_3]});

void ioInit()
{
    Serial.println("Mini synth init");
    display.init();

    audioInit();
    midiInit();

    display.update();
    tempoInit();
}

void ioLoop()
{
    midiLoop();
    display.loop();
    tempoLoop();
}

#endif
