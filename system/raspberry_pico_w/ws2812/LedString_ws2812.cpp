#include "ws2812/LedString_ws2812.hpp"
#include "LedString_ws2812.hpp"
#include <stdio.h>

LedString_ws2812::LedString_ws2812(uint32_t pin, uint32_t pixelsNum)
    : leds(pin, pixelsNum, pio0, 0, WS2812::FORMAT_GRB)
{
    leds.fill(0);
}

void LedString_ws2812::update()
{
    leds.show();
}

uint64_t LedString_ws2812::ledsCount() const
{
    return leds.size();
}

YALC::ColorOrder LedString_ws2812::colorOrder() const
{
    return YALC::ColorOrder::RGB;
}

void LedString_ws2812::setColors(uint32_t startPixel, byte *data, uint32_t numberOfPixels)
{
    for(int i = 0; i < numberOfPixels; ++i)
    {
        printf("LedString_ws2812::setColors id %d r %d g %d b %d\n", startPixel + i, data[3*i + 0], data[3*i + 1], data[3*i + 2]);
        leds.setPixelColor(startPixel + i, data[3*i + 0], data[3*i + 1], data[3*i + 2]);
    }
}
