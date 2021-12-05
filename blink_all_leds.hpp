#pragma once

#include "effect_information.hpp"

class blink_all_leds {
public:
    effect_information get_info();

    void init(Adafruit_NeoPixel& pixels);
    void periodic(int64_t time_elapsed);
    void deinit();

    Adafruit_NeoPixel* pixels;
    int64_t counter;
    bool on;
};