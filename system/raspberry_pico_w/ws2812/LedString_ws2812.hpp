#pragma once

#include "Common.hpp"
#include "ws2812/ws2812.hpp"

class LedString_ws2812 {
    public:
        LedString_ws2812(uint32_t pin, uint32_t pixelsNum);

        void init();
        void update();

        //YALC specyfic functions
        uint64_t ledsCount() const;
        YALC::ColorOrder colorOrder() const;
        void setColors(uint32_t startPixel, byte* data, uint32_t numberOfPixels);

    private:
        WS2812 leds;
};