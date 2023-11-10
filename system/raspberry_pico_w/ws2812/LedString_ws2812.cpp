#include "ws2812/LedString_ws2812.hpp"
#include "LedString_ws2812.hpp"

LedString_ws2812::LedString_ws2812(uint32_t pin, uint32_t pixelsNum)
    : leds(pin, pixelsNum, pio0, 0)
{
}

uint64_t LedString_ws2812::ledsCount() const
{
    return leds.size();
}

YALC::ColorOrder LedString_ws2812::colorOrder() const
{
    return YALC::ColorOrder::GRB;
}

void LedString_ws2812::setColors(uint32_t startPixel, byte *data, uint32_t numberOfPixels)
{
    for(int i = 0; i < numberOfPixels; ++i)
    {
        leds.setPixelColor(startPixel + i, data[3*i + 0], data[3*i + 1], data[3*i + 2]);
    }
}