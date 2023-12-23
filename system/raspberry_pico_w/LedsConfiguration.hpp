#pragma once

#include "Application.hpp"
#include "Common.hpp"

enum class LedColorConfig {
    RGB,
    GRB,
    BGR
};

const char* to_char(LedColorConfig val);
YALC::ColorOrder to_colorOrder(LedColorConfig val);

struct LedConfig {
    uint32_t length;
    LedColorConfig colorConfig;
};

class LedsConfiguration {
public:
    LedsConfiguration(Application& app);
    bool init();

    uint32_t getPixelsFor(uint32_t pin);
    void setPixelsFor(uint32_t pin, uint32_t value);
    void setColorConfigFor(uint32_t pin, LedColorConfig value);

    uint32_t getNumberOfPins() const;
    LedColorConfig getColorConfig(uint32_t pin);

private:
    void storeConfig();
    Application app;

    LedConfig leds[2];
};