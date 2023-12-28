#pragma once

#include "Common.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class SFMLDisplay {
public:
    SFMLDisplay(int pixels, int width, int height, uint32_t wrapLeds);
    void periodic();
    bool isOk();

    //Methods from interface
    uint64_t ledsCount() const;
    YALC::ColorOrder colorOrder() const;
    void setColors(uint32_t startPixel, byte* data, uint32_t numberOfPixels);
    void setPixel(uint32_t pixel, uint8_t r, uint8_t g, uint8_t b);

private:
    std::vector<YALC::PixelData8Bit> pixels;
    int width;
    int height;
    uint32_t wrapLeds;


    sf::RenderWindow window;

    void drawPixels();
};