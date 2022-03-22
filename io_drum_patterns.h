#ifndef IO_DRUM_PATTERNS_H_
#define IO_DRUM_PATTERNS_H_

#include "Pattern.h"

#define DRUM_PATTERN_COUNT 4

Pattern drumPatterns[DRUM_PATTERN_COUNT] = {
    (Pat){{KICK}, {0}, {0}, {0}, {END}},
    (Pat){{KICK}, {0}, {HITHAT}, {0}, {END}},
    (Pat){{KICK}, {0}, {SNARE}, {0}, {END}},
    (Pat){{KICK}, {0}, {KICK}, {0}, {END}},
};

#endif
