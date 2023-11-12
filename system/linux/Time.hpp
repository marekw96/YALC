#pragma once

#include "Common.hpp"

class Duration {
public:
    Duration(uint64_t value);
    uint64_t asMiliseconds() const;
    uint64_t asSeconds() const;
    uint64_t asMicroseconds() const;

private:
    uint64_t value;
};

class TimePoint {
public:
    TimePoint(uint64_t timeValue);
    Duration operator-(const TimePoint& other);

private:
    uint64_t value;
};

class Time {
    public:
        TimePoint current();
};