#pragma once
#include <cstdint>

struct GnssFrame {
    uint64_t timestamp_ms;
    double lat;
    double lon;
    float hdop;
    int satellites_used;
    int fix_type;
};

class GnssProcessor {
public:
    GnssProcessor();

    // TODO: Implement this function
    // Returns true if the frame is valid and accepted, false if rejected (anomaly/error)
    bool process(const GnssFrame& frame);
};
