#pragma once

#include "Application.hpp"

class LedsConfiguration {
public:
    LedsConfiguration(Application& app);
    bool init();

    uint32_t getPixelsFor(uint32_t pin);
    void setPixelsFor(uint32_t pin, uint32_t value);
    uint32_t getNumberOfPins() const;

private:
    void storeConfig();
    Application app;

    uint32_t leds[2];
};