#ifndef IO_UTIL_H_
#define IO_UTIL_H_

#include <Arduino.h>

#define WAVEFORM_COUNT 9

float pctAdd(float val, int8_t direction) {
    return (float)(constrain(val * 127 + direction, 0, 127)) / 127;
}

int mod(int n, int m) { return ((n % m) + m) % m; }

#endif
