#ifndef IO_DISPLAY_HELPER_H_
#define IO_DISPLAY_HELPER_H_

const char* getFilter(byte w) {
    switch (w) {
        case 0:
            return "LowP";
        case 1:
            return "BandP";
        case 2:
            return "HighP";
        case 3:
            return "None";
    }
    return "??";
}

const char* getWave(byte w) {
    if (w < WAVEFORM_COUNT) {
        switch (w) {
            case 0:
                return "Sine";
            case 1:
                return "Sawtooth";
            case 2:
                return "Square";
            case 3:
                return "Triangle";
            case 4:
                return "Arbitrary";
            case 5:
                return "Pulse";
            case 6:
                return "Saw reverse";
            case 7:
                return "Sample hold";
            case 8:
                return "Tri var";
        }
    }
    return "unknown";
}

#endif
