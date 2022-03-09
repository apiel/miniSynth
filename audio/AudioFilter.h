#ifndef AudioFilter_h_
#define AudioFilter_h_

#include <Arduino.h>
#include <Audio.h>

#include "../io_util.h"
#include "./audio_dumb.h"

#define FILTER_TYPE_COUNT 4

const static uint32_t PROGMEM FILTERFREQS256[256] = {
    20, 21, 22, 23, 24, 26, 27, 28, 29, 31, 32,
    33, 34, 35, 36, 38, 39, 40, 41, 43, 44, 46,
    47, 49, 50, 52, 53, 55, 57, 59, 61, 63, 65,
    68, 70, 73, 76, 78, 81, 84, 87, 91, 94, 98,
    102, 106, 110, 114, 119, 123, 128, 133, 138, 144, 149,
    155, 161, 167, 174, 181, 187, 195, 202, 210, 218, 226,
    234, 243, 252, 261, 271, 281, 291, 301, 312, 323, 334,
    346, 358, 370, 383, 395, 409, 422, 436, 451, 465, 480,
    496, 512, 528, 544, 561, 578, 596, 614, 633, 652, 671,
    691, 711, 732, 753, 774, 796, 818, 841, 865, 888, 913,
    937, 962, 988, 1014, 1041, 1068, 1096, 1124, 1152, 1182, 1211,
    1242, 1272, 1304, 1335, 1368, 1401, 1434, 1468, 1503, 1538, 1574,
    1610, 1647, 1684, 1722, 1761, 1800, 1840, 1881, 1922, 1964, 2006,
    2049, 2093, 2137, 2182, 2227, 2274, 2320, 2368, 2416, 2465, 2515,
    2565, 2616, 2668, 2720, 2773, 2827, 2881, 2936, 2992, 3049, 3106,
    3164, 3223, 3283, 3343, 3404, 3466, 3529, 3592, 3656, 3721, 3787,
    3853, 3921, 3989, 4058, 4127, 4198, 4269, 4341, 4414, 4488, 4563,
    4638, 4715, 4792, 4870, 4949, 5029, 5109, 5191, 5273, 5357, 5441,
    5526, 5612, 5699, 5787, 5876, 5965, 6056, 6147, 6240, 6333, 6427,
    6523, 6619, 6716, 6814, 6913, 7013, 7114, 7216, 7319, 7424, 7529,
    7635, 7742, 7850, 7959, 8069, 8180, 8292, 8405, 8519, 8634, 8751,
    8868, 8986, 9106, 9226, 9348, 9471, 9594, 9719, 9845, 9972, 10100,
    10230, 10360, 10491, 10624, 10758, 10893, 11029, 11166, 11304, 11443, 11584,
    11726, 11869, 12000};

const static float POWER[128] = {
    0.0f, 0.00009f, 0.0003f, 0.0006f, 0.001f, 0.0016f, 0.0022f, 0.003f,
    0.004f, 0.005f, 0.0062f, 0.0075f, 0.0089f, 0.0105f, 0.0122f, 0.014f,
    0.016f, 0.018f, 0.02f, 0.022f, 0.025f, 0.027f, 0.03f, 0.033f,
    0.036f, 0.039f, 0.042f, 0.045f, 0.049f, 0.052f, 0.056f, 0.06f,
    0.063f, 0.068f, 0.072f, 0.076f, 0.08f, 0.085f, 0.09f, 0.094f,
    0.099f, 0.104f, 0.109f, 0.115f, 0.12f, 0.126f, 0.131f, 0.137f,
    0.143f, 0.149f, 0.155f, 0.161f, 0.168f, 0.174f, 0.181f, 0.188f,
    0.194f, 0.201f, 0.209f, 0.216f, 0.223f, 0.231f, 0.238f, 0.246f,
    0.254f, 0.262f, 0.27f, 0.278f, 0.287f, 0.295f, 0.304f, 0.313f,
    0.321f, 0.33f, 0.34f, 0.349f, 0.358f, 0.368f, 0.377f, 0.387f,
    0.397f, 0.407f, 0.417f, 0.427f, 0.437f, 0.448f, 0.459f, 0.469f,
    0.48f, 0.491f, 0.502f, 0.513f, 0.525f, 0.536f, 0.548f, 0.56f,
    0.571f, 0.583f, 0.595f, 0.608f, 0.62f, 0.632f, 0.645f, 0.658f,
    0.671f, 0.684f, 0.697f, 0.71f, 0.723f, 0.737f, 0.75f, 0.764f,
    0.778f, 0.792f, 0.806f, 0.82f, 0.834f, 0.849f, 0.863f, 0.878f,
    0.893f, 0.908f, 0.923f, 0.938f, 0.953f, 0.969f, 0.984f, 1.0f};

// some stuff
// https://github.com/vinniefalco/DSPFilters/tree/master/shared/DSPFilters/source
// https://github.com/corbanbrook/dsp.js/

class AudioFilter : public AudioDumb
{
private:
    // AudioMixer4 mixer;
    // AudioSynthWaveformDc dc;
    // AudioFilterStateVariable filter;
    AudioFilterBiquad filter;
    // AudioConnection *patchCordFilter[FILTER_TYPE_COUNT];
    AudioConnection *patchCordInput;
    AudioConnection *patchCordOutput;
    // AudioConnection *patchCordDcToFilter;

public:
    AudioDumb input;
    AudioEffectEnvelope env;

    byte cutoffPos = 127;
    float cutoff = 0.0;
    byte resonancePos = 127;
    float resonance = 1.0;
    byte currentFilter = 0;

    AudioFilter()
    {
        patchCordInput = new AudioConnection(input, filter);
        // patchCordFilter[0] = new AudioConnection(filter, 0, mixer, 0);
        // patchCordFilter[1] = new AudioConnection(filter, 1, mixer, 1);
        // patchCordFilter[2] = new AudioConnection(filter, 2, mixer, 2);
        // patchCordFilter[3] = new AudioConnection(input, 0, mixer, 3);
        // patchCordOutput = new AudioConnection(mixer, *this);
        // patchCordDcToFilter = new AudioConnection(dc, 0, filter, 1);

        patchCordOutput = new AudioConnection(filter, *this);

        // setCurrentFilter(1);
        setCutoff(cutoffPos);
        setResonance(resonancePos);

        // dc.amplitude(0.0);
    }

    // void setCurrentFilter(int8_t direction)
    // {
    //     currentFilter = mod(currentFilter + direction, FILTER_TYPE_COUNT);

    //     mixer.gain(0, 0.0);
    //     mixer.gain(1, 0.0);
    //     mixer.gain(2, 0.0);
    //     mixer.gain(3, 0.0);

    //     switch (currentFilter)
    //     {
    //     case 0:
    //         mixer.gain(0, 1.0);
    //         break;
    //     case 1:
    //         mixer.gain(1, 1.0);
    //         break;
    //     case 2:
    //         mixer.gain(2, 1.0);
    //         break;
    //     default:
    //         mixer.gain(3, 1.0);
    //         break;
    //     }
    // }

    void setCutoff(byte value)
    {
        cutoffPos = value * 2;
        cutoff = FILTERFREQS256[cutoffPos];
        filter.setLowpass(0, cutoff, resonance);

        // filter.frequency(cutoff);

        // if (cutoff <= 2000)
        // {
        //     filter.octaveControl(
        //         4.0f + ((2000.0f - cutoff) / 710.0f)); // More bass
        // }
        // else if (cutoff > 2000 && cutoff <= 3500)
        // {
        //     filter.octaveControl(3.0f + ((3500.0f - cutoff) /
        //                                  1500.0f)); // Sharper cutoff
        // }
        // else if (cutoff > 3500 && cutoff <= 7000)
        // {
        //     filter.octaveControl(2.0f + ((7000.0f - cutoff) /
        //                                  4000.0f)); // Sharper cutoff
        // }
        // else
        // {
        //     filter.octaveControl(1.0f + ((12000.0f - cutoff) /
        //                                  5100.0f)); // Sharper cutoff
        // }
    }

    void setResonance(byte value)
    {
        // resonancePos = value;
        // filter.resonance((13.9f * POWER[resonancePos]) + 1.1f);
        resonancePos = value;
        // resonance = ((float)resonancePos) / 127.0f;
        resonance = ((float)resonancePos) / 127.0f * 2;
        // resonance = (float)resonancePos;
        // filter.resonance((13.9f * ((float)resonance) / 127.0f) + 1.1f);
        filter.setLowpass(0, cutoff, resonance);
    }
};

#endif
