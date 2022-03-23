#ifndef IO_DRUM_PATTERNS_H_
#define IO_DRUM_PATTERNS_H_

#include "Pattern.h"

#define DRUM_PATTERN_COUNT 7

// let start at 23 just because :p but also because END is 1 and 0 is used for pause
#define KICK 23
#define SNARE 24
#define HITHAT 25
#define MISC 26

Pattern drumPatterns[] = {
    (Pat){{KICK}, {0}, {0}, {0}, {END}},
    (Pat){{KICK}, {0}, {HITHAT}, {0}, {END}},
    (Pat){{KICK}, {0}, {SNARE}, {0}, {END}},
    (Pat){{KICK}, {0}, {KICK}, {0}, {END}},
    (Pat){{KICK}, {0}, {MISC}, {0}, {END}},
    (Pat){{KICK}, {MISC}, {MISC}, {0}, {END}},
    (Pat){{MISC}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {END}},
};

#endif
