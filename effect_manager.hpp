#pragma once
#include <Adafruit_NeoPixel.h>

#include "effect_information.hpp"

class effect_manager {
public:
    effect_manager();
    effect_information& get_current_effect();
    uint64_t get_number_of_effects();
    effect_information* get_effects();
    void set_pixels(Adafruit_NeoPixel& pixels);
    bool change_effect_to(const char* effect_name);

private:
    effect_information* effects;
    int number_of_effects;
    effect_information* current_effect;
    Adafruit_NeoPixel* pixels;
};