#pragma once

#include "Common.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class SFMLDisplay {
public:
    SFMLDisplay(int pixels, int width, int height);
    void periodic();
    bool isOk();

    //Methods from interface
    uint64_t ledsCount() const;
    YALC::ColorOrder colorOrder() const;
    void setColors(uint32_t startPixel, byte* data, uint32_t numberOfPixels);

private:
    std::vector<YALC::PixelData8Bit> pixels;
    int width;
    int height;


    sf::RenderWindow window;

    void drawPixels();
};