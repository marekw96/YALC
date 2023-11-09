#include "Time.hpp"

#include <pico/stdlib.h>

TimePoint Time::current()
{
    return TimePoint(time_us_64());
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
