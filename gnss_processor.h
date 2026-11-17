#pragma once
#include <cstdint>

struct GnssFrame {
    uint64_t timestamp_ms; // Time in milliseconds
    double lat;            // Latitude
    double lon;            // Longitude
    float hdop;            // Horizontal Dilution of Precision
    int satellites_used;   // Number of satellites used for estimation
    int fix_type;          // 0 = No Fix, 1 = Standard GPS, 2 = Mixed
};

class GnssProcessor {
public:
    GnssProcessor();

    // Returns true if the frame is valid and accepted, false if rejected
    bool process(const GnssFrame& frame);

private:
    bool has_previous_;
    uint64_t last_timestamp_ms_;
};
