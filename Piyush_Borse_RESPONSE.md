# Piyush — GNSS Processor Response

## 1. What assumptions did you have to make?

- **HDOP threshold = 5.0** — HDOP above 5 is generally considered poor quality in GNSS. The test uses 6.5 as a reject case, so 5.0 is a safe cutoff.
- **Minimum 4 satellites** — You need at least 4 satellites for a 3D position fix. Test confirms this (3 satellites → reject).
- **Maximum gap = 1500 ms** — At 10 Hz, frames come every 100 ms. A 1.5 second gap means 15 missed frames, which is a significant outage. Test confirms this (1600 ms gap → reject).
- **fix_type 2 (Mixed) is valid** — Only fix_type 0 means "No Fix". Mixed GPS (type 2) is usable.
- **Rejected frames don't update state** — Timing checks compare against the last *accepted* frame so one bad frame doesn't cause the next good frame to also fail.

## 2. Which validation rules did you implement?

Five checks, in order:

1. **Fix type** — reject if `fix_type == 0` (no fix)
2. **Coordinate range** — latitude must be [-90, 90], longitude must be [-180, 180]
3. **HDOP** — reject if `hdop > 5.0`
4. **Satellite count** — reject if `satellites_used < 4`
5. **Timestamp** — must be strictly increasing, and gap from last accepted frame must be ≤ 1500 ms

For accepted frames, the position is printed to stdout.

## 3. How should the processor behave after a rejected frame, and why?

A rejected frame is ignored — the internal state (last timestamp) is **not updated**. This way, the next valid frame is compared against the last good frame, not the bad one.

If we updated state on rejection, a single bad frame could cause the next good frame to also fail (e.g., the timestamp gap would be measured wrong), creating a chain of unnecessary rejections.

**Trade-off:** After many consecutive rejections (like the satellite dropout in the CSV data), the gap keeps growing and all subsequent frames get rejected too. In a real system, you'd add a reset mechanism after prolonged outage.

## 4. What would you change before using this component on a real robot?

- **Add a reset/re-acquisition** — After many consecutive rejections, clear the state and treat the next valid frame as a fresh start
- **Make thresholds configurable** — Pass HDOP limit, satellite minimum, gap limit as parameters instead of hardcoded values
- **Use callbacks instead of cout** — Replace printing with a callback or publish to a ROS topic so other modules can receive position updates
- **Fuse with IMU** — Raw GNSS alone isn't enough; use an Extended Kalman Filter to combine GNSS with IMU data
- **Add thread safety** — Protect state with a mutex if called from multiple threads

## 5. AI Usage

I used an AI assistant for research (understanding GNSS thresholds, HDOP ranges, satellite requirements) and for drafting this documentation. The code and design decisions are my own.

## Build & Run

```bash
g++ -std=c++17 test_gnss_processor.cpp gnss_processor.cpp -o test_gnss
./test_gnss

g++ -std=c++17 csv_replay.cpp gnss_processor.cpp -o csv_replay
./csv_replay gnss.csv
```
