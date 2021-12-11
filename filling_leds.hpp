#pragma once

#include "effect_information.hpp"
#include "rgb.hpp"

class filling_leds {
public:
    effect_information get_info();

    void init(Adafruit_NeoPixel& pixels);
    void periodic(int64_t time_elapsed);
    void deinit();

    Adafruit_NeoPixel* pixels;
    int64_t counter;
    RGB first_color;
    RGB second_color;
    int current_position;
    int filled;
};