#include "ws2812/LedString_ws2812.hpp"
#include "LedString_ws2812.hpp"
#include <stdio.h>

PIO PIN_PIO[] = {pio0, pio1};

LedString_ws2812::LedString_ws2812(uint32_t pin, uint32_t pixelsNum, YALC::ColorOrder colorConfig)
    : leds(pin, pixelsNum, PIN_PIO[pin], pin, WS2812::FORMAT_GRB), lastUpdate(Time{}.current())
{
    this->colorConfig = colorConfig;
}

void LedString_ws2812::init() {
    leds.fill(0);
}

void LedString_ws2812::update()
{
    auto now = Time{}.current();
    if((now - lastUpdate).asMiliseconds() > 33){
        leds.show();
        lastUpdate = now;
    }
}

uint64_t LedString_ws2812::ledsCount() const
{
    return leds.size();
}

YALC::ColorOrder LedString_ws2812::colorOrder() const
{
    return colorConfig;
}

void LedString_ws2812::setColors(uint32_t startPixel, byte *data, uint32_t numberOfPixels)
{
    for(int i = 0; i < numberOfPixels; ++i)
    {
        leds.setPixelColor(startPixel + i, data[3*i + 0], data[3*i + 1], data[3*i + 2]);
    }
}

void LedString_ws2812::setPixel(uint32_t pixel, uint8_t r, uint8_t g, uint8_t b)
{
    switch(colorConfig){
        case YALC::ColorOrder::RGB:
            return leds.setPixelColor(pixel, r, g, b);
        case YALC::ColorOrder::BGR:
            return leds.setPixelColor(pixel, b, g, r);
        case YALC::ColorOrder::GRB:
        default:
            return leds.setPixelColor(pixel, g, r, b);
    }
}
