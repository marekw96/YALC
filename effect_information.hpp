#pragma once

#include <Adafruit_NeoPixel.h>
#include <functional>

#include "effect_parameter.hpp"
#include "view.hpp"

struct effect_information {
    const char* id;
    const char* name;
    const char* description;
    std::function<void(Adafruit_NeoPixel&)> init;
    std::function<void(int64_t)> periodic;
    std::function<void(void)> deinit;
    std::function<bool(const String&, const String&)> set_parameter;
    std::function<String(const String&)> get_parameter;
    view<effect_parameter> parameters;
};