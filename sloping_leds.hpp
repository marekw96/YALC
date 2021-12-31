#pragma once

#include "effect_information.hpp"
#include "rgb.hpp"

class sloping_leds {
public:
    effect_information get_info();

    void init(Adafruit_NeoPixel& pixels);
    void periodic(int64_t time_elapsed);
    void deinit();
    bool set_parameter(const String& name, const String& value);
    String get_parameter(const String& name);

private:
    struct slope{int position; RGB color; };

    Adafruit_NeoPixel* pixels;
    int64_t counter;
    bool on;
    RGB color;
    int position;
    slope* slopes = nullptr;
    int num_of_slopes;
    uint64_t timeout_ns;
    bool change_colors;
    uint8_t last_used_color;
};