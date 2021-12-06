#pragma once

#include "effect_information.hpp"

struct RGB
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
};

class brethe_colors {
public:
    effect_information get_info();

    void init(Adafruit_NeoPixel& pixels);
    void periodic(int64_t time_elapsed);
    void deinit();

    Adafruit_NeoPixel* pixels;
    int64_t counter;
    float current_color_R, current_color_G, current_color_B;
    RGB first_color;
    RGB second_color;
    uint8_t step_counter;
};