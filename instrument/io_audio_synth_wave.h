#ifndef IO_AUDIO_SYNTH_WAVE_H_
#define IO_AUDIO_SYNTH_WAVE_H_

#include <Arduino.h>
#include <Audio.h>

#include "../audio/audio_dumb.h"
#include "../audio/note.h"
#include "../io_util.h"
#include "../wavetable/AudioWaveTable.h"
#include "../wavetable/AudioWaveTableList.h"

class IO_AudioSynthWave : public AudioDumb {
   public:
    AudioWaveTable waveTable;
    AudioSynthWaveformModulated waveForm;
    AudioWaveTableList waveList;
    AudioDumb input;

    uint16_t currentWave = 0;
    char* waveName;

    float frequency = NOTE_FREQ[_C4];
    float amplitude = 1.0;

    AudioConnection* patchCordInputToWaveTable;
    AudioConnection* patchCordWaveTableToDumb;

    AudioConnection* patchCordInputToWaveForm;
    AudioConnection* patchCordWaveFormToDumb;

    IO_AudioSynthWave() {
        patchCordInputToWaveTable = new AudioConnection(input, waveTable);
        patchCordWaveTableToDumb = new AudioConnection(waveTable, *this);

        patchCordInputToWaveForm = new AudioConnection(input, waveForm);
        patchCordWaveFormToDumb = new AudioConnection(waveForm, *this);

        setFrequency(0);
        setAmplitude(0);
        setNextWaveform(0);

        waveForm.arbitraryWaveform(waveList.getTable(0)->table, 172.0);
        waveForm.begin(0);
    }

    IO_AudioSynthWave(float freq) {
        frequency = freq;
        IO_AudioSynthWave();
    }

    bool isWaveForm() { return currentWave < WAVEFORM_COUNT; }
    bool isWaveArbitrary() {
        return !isWaveForm() &&
               currentWave < AUDIO_WAVETABLE_SIZE + WAVEFORM_COUNT;
    }
    bool isWaveTable() { return !isWaveForm() && !isWaveArbitrary(); }

    IO_AudioSynthWave* setStart(int8_t direction) {
        waveTable.setStart(waveTable.start + direction);
        return this;
    }

    IO_AudioSynthWave* setFrequency(int8_t direction) {
        frequency =
            constrain(frequency + direction, 0, AUDIO_SAMPLE_RATE_EXACT / 2);
        waveForm.frequency(frequency);
        waveTable.setFrequency(frequency);
        return this;
    }

    IO_AudioSynthWave* setAmplitude(int8_t direction) {
        amplitude = pctAdd(amplitude, direction);
        waveForm.amplitude(amplitude);
        waveTable.setAmplitude(amplitude);
        return this;
    }

    IO_AudioSynthWave* setNextWaveform(int8_t direction) {
        currentWave = mod(currentWave + direction,
                          (AUDIO_WAVETABLE_SIZE * 2) + WAVEFORM_COUNT);
        if (isWaveForm()) {
            waveForm.begin(currentWave);
            Serial.printf("Set basic wave form %d\n", currentWave);
        } else if (isWaveArbitrary()) {
            uint16_t pos = currentWave - WAVEFORM_COUNT;
            waveName = (char*)waveList.getTable(pos)->name;
            Serial.printf("Set arbitrary wave form %d %s\n", pos, waveName);
            waveForm.arbitraryWaveform(waveList.getTable(pos)->table, 172.0);
            waveForm.begin(WAVEFORM_ARBITRARY);
        } else {
            uint16_t pos =
                currentWave - (AUDIO_WAVETABLE_SIZE + WAVEFORM_COUNT);
            waveName = (char*)waveList.getTable(pos)->name;
            Serial.printf("Set wave table %d %s\n", pos, waveName);
            waveTable.setTable(waveList.getTable(pos)->table,
                               waveList.getTable(pos)->size);
        }
        applyCord();
        return this;
    }

   private:
    void applyCord() {
        // will use something else
        if (isWaveForm() || isWaveArbitrary()) {
            patchCordInputToWaveTable->disconnect();
            patchCordWaveTableToDumb->disconnect();
            patchCordInputToWaveForm->connect();
            patchCordWaveFormToDumb->connect();
        } else {
            patchCordInputToWaveForm->disconnect();
            patchCordWaveFormToDumb->disconnect();
            patchCordInputToWaveTable->connect();
            patchCordWaveTableToDumb->connect();
        }
    }
};

#endif
