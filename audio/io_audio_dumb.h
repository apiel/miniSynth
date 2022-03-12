#ifndef IO_AUDIO_DUMB_H_
#define IO_AUDIO_DUMB_H_

#include <Arduino.h>
#include <AudioStream.h>

class IO_AudioDumb : public AudioStream {
   public:
    IO_AudioDumb(void) : AudioStream(1, inputQueueArray) {}
    virtual void update(void) {
        audio_block_t *block = receiveReadOnly();
        if (!block) return;
        transmit(block);
        release(block);
    }

   protected:
    audio_block_t *inputQueueArray[1];
};

#endif
