#pragma once
#include <chrono>

/* Basic "stopwatch" class to compare the time difference in seconds between when
 * the Timer was reset and when getTimeDifference was called.
 */
class SimpleTimer
{
public:
    SimpleTimer();
    ~SimpleTimer() = default;

    void reset();
    float getTimeDifference() const;

private:
    std::chrono::time_point<std::chrono::system_clock> start;
};
