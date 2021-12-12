#pragma once

#include "effect_information.hpp"
#include "rgb.hpp"

class brethe_colors {
public:
    brethe_colors();
    effect_information get_info();

    void init(Adafruit_NeoPixel& pixels);
    void periodic(int64_t time_elapsed);
    void deinit();
    bool set_parameter(const String& name, const String& value);
    String get_parameter(const String& name);

    Adafruit_NeoPixel* pixels;
    int64_t counter;
    RGB_t<float> step;
    int steps;
    RGB first_color;
    RGB second_color;
    uint8_t step_counter;
};