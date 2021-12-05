#pragma once

#include <Adafruit_NeoPixel.h>
#include <functional>

struct effect_information {
    const char* name;
    const char* description;
    std::function<void(Adafruit_NeoPixel&)> init;
    std::function<void(int64_t)> periodic;
    std::function<void(void)> deinit;
};