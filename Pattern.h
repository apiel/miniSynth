#ifndef PATTERN_H_
#define PATTERN_H_

#include "./Step.h"

#define MAX_STEPS_IN_PATTERN 32
#define PATTERN_NAME_SIZE 15

class Pattern {
   public:
    byte pos;
    byte stepCount = MAX_STEPS_IN_PATTERN;
    char name[PATTERN_NAME_SIZE];
    Step steps[MAX_STEPS_IN_PATTERN];

    Pattern() { setDefaultName(); }

    Pattern* set(Pattern* p) {
        stepCount = p->stepCount;
        pos = p->pos;
        setName(p->name);
        for (byte pos = 0; pos < MAX_STEPS_IN_PATTERN; pos++) {
            steps[pos].set(&p->steps[pos]);
        }
        return this;
    }

    Pattern* setDefaultName() {
        setName("New Pattern");
        return this;
    }

    Pattern* setName(const char* ptrName) {
        snprintf(name, PATTERN_NAME_SIZE, "%s", ptrName);
        return this;
    }

    Pattern* add(byte pos, byte note, byte duration, byte velocity) {
        return add(pos, note, duration, velocity, false);
    }

    Pattern* add(byte pos, byte note, byte duration, byte velocity,
                 bool slide) {
        if (pos < MAX_STEPS_IN_PATTERN) {
            steps[pos].set(note, duration, velocity, slide);
        }
        return this;
    }

    // for testing
    void print() {
        for (byte x = 0; x < stepCount; x++) {
            Serial.print("[");
            Serial.print(x);
            Serial.print(", ");
            Serial.print(steps[x].note);
            Serial.print(", ");
            Serial.print(steps[x].duration);
            Serial.print("],");
            if (x % 8 == 7) {
                Serial.println("");
            }
        }
        Serial.println("\n");
    }

    Pattern* remove(byte pos) {
        steps[pos].clear();
        return this;
    }

    Pattern* clear() {
        for (byte pos = 0; pos < MAX_STEPS_IN_PATTERN; pos++) {
            steps[pos].clear();
        }
        return this;
    }

    Pattern* repeat(byte pos, byte len) {
        return repeat(pos, len, (byte)stepCount / len);
    }

    Pattern* repeat(byte pos, byte len, byte count) {
        for (byte n = 1; n < count; n++) {
            for (byte p = 0; p < len; p++) {
                steps[pos + n * len + p] = steps[pos + p];
            }
        }
        return this;
    }
};

#endif
