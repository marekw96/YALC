#pragma once

#include "Common.hpp"
#include "ws2812/ws2812.hpp"
#include "Time.hpp"

class LedString_ws2812 {
    public:
        LedString_ws2812(uint32_t pin, uint32_t pixelsNum, YALC::ColorOrder colorConfig);

        void init();
        void update();

        //YALC specyfic functions
        uint64_t ledsCount() const;
        YALC::ColorOrder colorOrder() const;
        void setColors(uint32_t startPixel, byte* data, uint32_t numberOfPixels);
        void setPixel(uint32_t pixel, uint8_t r, uint8_t g, uint8_t b);

    private:
        WS2812 leds;
        TimePoint lastUpdate;
        YALC::ColorOrder colorConfig;
};