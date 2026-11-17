#include <cstdlib>
#include <iostream>

#include "gnss_processor.h"

using namespace std;

#define RUN_TEST(name, expr) \
    do { \
        if (expr) { \
            cout << "[PASS] " << name << endl; \
        } else { \
            cout << "[FAIL] " << name << endl; \
            return EXIT_FAILURE; \
        } \
    } while (0)

int main() {
    // ---- Original Tests (1-7) ----

    // Test 1: Normal frames should succeed.
    {
        GnssProcessor p;
        bool r1 = p.process({1000, 12.971600, 77.594600, 1.2f, 8, 1});
        bool r2 = p.process({1100, 12.971601, 77.594601, 1.1f, 9, 1});
        RUN_TEST("Normal Operation", r1 && r2);
    }

    // Test 2: HDOP above some threshold should fail.
    {
        GnssProcessor p;
        bool r = p.process({1000, 12.9716, 77.5946, 6.5f, 8, 1});
        RUN_TEST("High HDOP Rejection", !r);
    }

    // Test 3: A missing fix should fail.
    {
        GnssProcessor p;
        bool r = p.process({1000, 12.9716, 77.5946, 1.2f, 8, 0});
        RUN_TEST("No Fix Rejection", !r);
    }

    // Test 4: Coordinates outside the valid GNSS range should fail.
    {
        GnssProcessor p;
        bool r = p.process({1000, 91.0, 77.5946, 1.2f, 8, 1});
        RUN_TEST("Out-of-Range Coordinate Rejection", !r);
    }

    // Test 5: Fewer than four satellites should fail.
    {
        GnssProcessor p;
        bool r = p.process({1000, 12.9716, 77.5946, 1.2f, 3, 1});
        RUN_TEST("Low Satellite Rejection", !r);
    }

    // Test 6: A gap longer than 1.5 seconds should fail.
    {
        GnssProcessor p;
        bool r1 = p.process({1000, 12.9716, 77.5946, 1.2f, 8, 1});
        bool r2 = p.process({2600, 12.9717, 77.5947, 1.2f, 8, 1});
        RUN_TEST("Timestamp Gap Rejection", r1 && !r2);
    }

    // Test 7: Timestamps must be strictly increasing.
    {
        GnssProcessor p;
        bool r1 = p.process({1000, 12.9716, 77.5946, 1.2f, 8, 1});
        bool r2 = p.process({1000, 12.9717, 77.5947, 1.2f, 8, 1});
        RUN_TEST("Non-Monotonic Timestamp Rejection", r1 && !r2);
    }

    // ---- Additional Tests (8-11) ----

    // Test 8: Rejected frame should not block the next valid frame.
    //         Bad frame in the middle, good frame after it should still pass.
    {
        GnssProcessor p;
        bool r1 = p.process({1000, 12.9716, 77.5946, 1.2f, 8, 1});  // good
        bool r2 = p.process({1100, 12.9716, 77.5946, 8.0f, 8, 1});  // bad HDOP
        bool r3 = p.process({1200, 12.9717, 77.5947, 1.2f, 8, 1});  // good
        RUN_TEST("Rejected Frame Does Not Block Next", r1 && !r2 && r3);
    }

    // Test 9: fix_type=2 (Mixed) should be accepted — only 0 means no fix.
    {
        GnssProcessor p;
        bool r = p.process({1000, 12.9716, 77.5946, 1.2f, 8, 2});
        RUN_TEST("Mixed Fix Type Accepted", r);
    }

    // Test 10: Longitude out of range (< -180) should fail.
    {
        GnssProcessor p;
        bool r = p.process({1000, 0.0, -180.1, 1.2f, 8, 1});
        RUN_TEST("Out-of-Range Longitude Rejection", !r);
    }

    // Test 11: Backward timestamp (goes back in time) should fail.
    {
        GnssProcessor p;
        bool r1 = p.process({2000, 12.9716, 77.5946, 1.2f, 8, 1});
        bool r2 = p.process({1500, 12.9717, 77.5947, 1.2f, 8, 1});
        RUN_TEST("Backward Timestamp Rejection", r1 && !r2);
    }

    cout << "\nAll tests passed successfully!" << endl;
    return EXIT_SUCCESS;
}
