#include "gnss_processor.h"
#include <iostream>
#include <iomanip>

using namespace std;

GnssProcessor::GnssProcessor()
    : has_previous_(false), last_timestamp_ms_(0) {}

bool GnssProcessor::process(const GnssFrame& frame) {

    // 1. Must have a fix (fix_type != 0)
    if (frame.fix_type == 0) {
        return false;
    }

    // 2. Latitude must be in [-90, 90], longitude in [-180, 180]
    if (frame.lat < -90.0 || frame.lat > 90.0 ||
        frame.lon < -180.0 || frame.lon > 180.0) {
        return false;
    }

    // 3. HDOP must be reasonable (at most 5.0)
    if (frame.hdop > 5.0f) {
        return false;
    }

    // 4. Need at least 4 satellites for a 3D fix
    if (frame.satellites_used < 4) {
        return false;
    }

    // 5. Timestamp checks (only if we have a previous accepted frame)
    if (has_previous_) {
        // Must be strictly increasing
        if (frame.timestamp_ms <= last_timestamp_ms_) {
            return false;
        }
        // Gap must not exceed 1500 ms (sensor runs at 10 Hz)
        uint64_t gap = frame.timestamp_ms - last_timestamp_ms_;
        if (gap > 1500) {
            return false;
        }
    }

    // Frame accepted — update state and print position
    has_previous_ = true;
    last_timestamp_ms_ = frame.timestamp_ms;

    cout << fixed << setprecision(6)
         << "[GNSS] t=" << frame.timestamp_ms << " ms"
         << "  lat=" << frame.lat
         << "  lon=" << frame.lon
         << endl;

    return true;
}
