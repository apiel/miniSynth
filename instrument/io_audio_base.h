#ifndef IO_AUDIO_BASE_H_
#define IO_AUDIO_BASE_H_

#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Audio.h>

template<class AudioCore = void, class AudioCoreUI = void>
class IO_AudioBase {
   protected:
    enum { VIEW_CORE, VIEW_SEQ, VIEW_COUNT };
    byte currentView = VIEW_CORE;

   public:
    AudioCoreUI* coreUI;

    void init() {  }

    void display(Adafruit_SSD1306* d) {
        switch (currentView) {
            // case VIEW_SEQ:
            //     seqUI->display(d);
            //     break;

            default:
                coreUI->display(d);
                break;
        }
        // display playing status on all the views
        // seqUI->displayPlayingStatus(d);
    }

    void setCurrentView(int8_t direction) {
        currentView = mod(currentView + direction, VIEW_COUNT);
        // Serial.printf("Current view %d\n", currentView);
    }

    void noteOnHandler(byte channel, byte note, byte velocity) {
        if (note == 18 || note == 42) {
            setCurrentView(-1);
        } else if (note == 19 || note == 43) {
            setCurrentView(1);
        } else if (note == 22 || note == 46) {
            // seq->toggle();
        } else {
            switch (currentView) {
                // case VIEW_SEQ:
                //     seqUI->noteOnHandler(channel, note, velocity);
                //     break;

                case VIEW_CORE:
                    coreUI->noteOnHandler(channel, note, velocity);
                    break;
            }
        }
    }

    void noteOffHandler(byte channel, byte note, byte velocity) {
        coreUI->noteOffHandler(channel, note, velocity);
    }

    void controlChangeHandler(byte channel, byte knob, int8_t direction) {
        switch (currentView) {
            // case VIEW_SEQ:
            //     seqUI->controlChangeHandler(channel, knob, direction);
            //     break;

            case VIEW_CORE:
                coreUI->controlChangeHandler(channel, knob, direction);
                break;
        }
    }
};

#endif
