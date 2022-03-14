#ifndef IO_AUDIO_SYNTH_H_
#define IO_AUDIO_SYNTH_H_

#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Audio.h>

#include "../io_audio_base.h"
#include "./io_audio_synth_core.h"
#include "./io_audio_synth_ui.h"
#include "../io_audio_loop.h"

class IO_AudioSynth
    : public IO_AudioSynthCore,
      public IO_AudioBase<IO_AudioSynthCore, IO_AudioSynthCoreUI> {
   public:
    IO_AudioSynth() {
        coreUI = new IO_AudioSynthCoreUI(this);
        coreUI->setBase(this);
        loop = new IO_AudioLoop<IO_AudioSynthCore>(this);
    }
};

#endif
