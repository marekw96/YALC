#include "Time.hpp"

#include <chrono>

TimePoint Time::current()
{
    auto point = std::chrono::high_resolution_clock::now();
    auto epch = point.time_since_epoch();
    auto microsec = std::chrono::duration_cast<std::chrono::microseconds>(epch).count();
    return TimePoint(microsec);
}

TimePoint::TimePoint(uint64_t timeValue)
    : value(timeValue)
{}

Duration TimePoint::operator-(const TimePoint &other)
{
    return Duration(value - other.value);
}

Duration::Duration(uint64_t value)
    : value(value)
{}

uint64_t Duration::asMiliseconds() const
{
    return value/1000u;
}

uint64_t Duration::asSeconds() const
{
    return value/1000000u;
}

uint64_t Duration::asMicroseconds() const
{
    return value;
}
