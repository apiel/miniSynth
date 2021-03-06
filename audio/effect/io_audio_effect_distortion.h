// See
// https://stackoverflow.com/questions/22312841/waveshaper-node-in-webaudio-how-to-emulate-distortion/22313408#22313408
// https://tonejs.github.io/docs/14.7.77/Distortion
// https://github.com/Tonejs/Tone.js/blob/dev/Tone/effect/Distortion.ts

#ifndef IO_AudioEffectDistortion_h_
#define IO_AudioEffectDistortion_h_

#include <Arduino.h>
#include <Audio.h>
#include <arm_math.h>

#define WAVESHAPE_SIZE 4097

class IO_AudioEffectDistortion : public AudioEffectWaveshaper {
   public:
    float amount = 50;
    float range = 20.0;

    IO_AudioEffectDistortion(void) { distortion(50); };

    void distortion(float _amount) {
        amount = _amount;
        if (amount > 0) {
            float deg = PI / 180;
            for (u_int16_t i = 0; i < WAVESHAPE_SIZE; i++) {
                float x = (float)i * 2.0 / (float)WAVESHAPE_SIZE - 1.0;
                waveshapeData[i] =
                    (3 + amount) * x * range * deg / (PI + amount * abs(x));
            }
            shape(waveshapeData, WAVESHAPE_SIZE);
        }
    }

    void setRange(float _range) {
        range = _range;
        distortion(amount);
    }

   private:
    float waveshapeData[WAVESHAPE_SIZE];
};

#undef WAVESHAPE_SIZE

#endif
