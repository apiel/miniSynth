#ifndef IO_ARP_PATTERNS_H_
#define IO_ARP_PATTERNS_H_

#include "Pattern.h"

#define PATTERN_COUNT 2

Pattern patterns[PATTERN_COUNT] = {
    (Pat){{_C4}, {0}, {0}, {0}, {END}},
    (Pat){{_C3, SLIDE}, {_C3}, {0}, {_C3}, {END}},
};

// void showPatterns()
// {
//     for (byte pos = 0; pos < PATTERN_COUNT; pos++)
//     {
//         Serial.printf("pattern %d:\n", pos);
//         patterns[pos].print();
//     }
// }

#endif
