#ifndef IO_CORE_H_
#define IO_CORE_H_

#include <Audio.h>

#include "io_display.h"
#include "audio/io_audio_synth.h"
#include "io_audio_loop.h"
#include "io_instrument_list.h"
#include "controller/akai_mpk_mini/io_controller_akai_mpk_mini.h"
#include "io_drum_machine.h"

IO_AudioSynth synths[SYNTH_COUNT];
IO_AudioSynth *synthsPtr[SYNTH_COUNT] = {&synths[SYNTH_0], &synths[SYNTH_1], &synths[SYNTH_2], &synths[SYNTH_3]};

IO_AudioLoop loop0(&synths[SYNTH_0], SYNTH_0);
IO_AudioLoop loop1(&synths[SYNTH_1], SYNTH_1);
IO_AudioLoop loop2(&synths[SYNTH_2], SYNTH_2);
IO_AudioLoop loop3(&synths[SYNTH_3], SYNTH_3);
IO_AudioLoop *loopsPtr[SYNTH_COUNT] = {&loop0, &loop1, &loop2, &loop3};

IO_ControllerAkaiMPKmini controller(&display, loopsPtr, synthsPtr);

IO_DrumMachine drumMachine;

AudioOutputMQS audioOut;
AudioMixer4 mixerOutput;
AudioMixer4 mixerSynth;

AudioConnection patchCordOutput(mixerOutput, audioOut);
AudioConnection patchCordMixerOutput0(mixerSynth, 0, mixerOutput, 0);
AudioConnection patchCordMixerOutput1(drumMachine.sample, 0, mixerOutput, 1);
AudioConnection patchCordSynth0(synths[SYNTH_0], 0, mixerSynth, 0);
AudioConnection patchCordSynth1(synths[SYNTH_1], 0, mixerSynth, 1);
AudioConnection patchCordSynth2(synths[SYNTH_2], 0, mixerSynth, 2);
AudioConnection patchCordSynth3(synths[SYNTH_3], 0, mixerSynth, 3);

#endif
