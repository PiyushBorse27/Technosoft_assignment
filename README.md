# Take-Home Assignment: GNSS Processor

Estimated time: 45-60 minutes

Goal: Implement a C++ class that processes GNSS data.

## Context

A mobile robot is navigating an outdoor route using its GNSS receiver for position updates. In open areas the data is usually reliable, but poor signal quality, missing fixes, or delayed frames can occur.

Your job is to implement the `GnssProcessor` class. Evaluate incoming frames, accept clean data, and log where useful.

## The Data Structure (`GnssFrame`)

Each incoming packet contains several fields. Use them as necessary.

```cpp
struct GnssFrame {
    uint64_t timestamp_ms; // Time in milliseconds
    double lat;            // Latitude
    double lon;            // Longitude
    float hdop;            // Horizontal Dilution of Precision
    int satellites_used;   // Number of satellites used for estimation
    int fix_type;          // 0 = No Fix, 1 = Standard GPS, 2 = Mixed
};
```

## Core Requirements

Your `GnssProcessor` class must implement the following primary function:

```cpp
bool process(const GnssFrame& frame);
```

As you process frames, your implementation must satisfy these operational requirements:

1. For valid frames, print the current position to standard output.
2. The sensor is expected to deliver data at **10 Hz**.
3. Pass all unit tests, then add at least **four meaningful
   tests** of your own.

## Notes

* Assume where necessary and inclue it in your report with explanation.
* Use C++17; External library use is discouraged, meaningful usage must be reasoned.
* Assignment does not require use of ROS.

## Extra Credit

Add a small reader to replay the recorded sensor log (`gnss.csv`) through your processor and report the behavior.

## Submission Requirements

Complete the assignment and Include a short `candidate_name_RESPONSE.md` that
answers the following:

1. What assumptions did you have to make?
2. Which validation rules did you implement?
3. How should the processor behave after a rejected frame, and why?
4. What would you change before using this component on a real robot?

> You may choose to prepare a single slide to explain your design and present it on the final interview.

## Helpers

`gnss_processor.h`: Skeleton of the `GnssProcessor` class.
`test_gnss_processor.cpp`: Starting unit-test suite.
`gnss.csv`: Optional recorded sensor log.

Compile and run using the following:

```bash
g++ -std=c++17 test_gnss_processor.cpp gnss_processor.cpp -o test_gnss
./test_gnss
```
