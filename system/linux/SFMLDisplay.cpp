#include "SFMLDisplay.hpp"



SFMLDisplay::SFMLDisplay(int pixels, int width, int height)
    :pixels(pixels), width(width), height(height),
    window(sf::VideoMode(width, height), "SFMLDisplay")
{

}

void SFMLDisplay::periodic()
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    window.clear();
    drawPixels();
    window.display();
}

bool SFMLDisplay::isOk()
{
    return window.isOpen();
}

uint64_t SFMLDisplay::ledsCount() const
{
    return pixels.size();
}

YALC::ColorOrder SFMLDisplay::colorOrder() const
{
    return YALC::ColorOrder::RGB;
}

void SFMLDisplay::setColors(uint32_t startPixel, byte *data, uint32_t numberOfPixels)
{
    for(int i = 0; i < numberOfPixels; ++i) {
        pixels[startPixel + i].data[0] = data[i*3 + 0];
        pixels[startPixel + i].data[1] = data[i*3 + 1];
        pixels[startPixel + i].data[2] = data[i*3 + 2];
    }
}

void SFMLDisplay::drawPixels()
{
    constexpr int perRow = 25;
    for(int i = 0; i < pixels.size(); ++i){
        sf::CircleShape shape(10.f);
        shape.setFillColor(sf::Color(pixels[i].data[0],pixels[i].data[1], pixels[i].data[2]));
        shape.setPosition((i%perRow)*22, (i/perRow)*22);
        window.draw(shape);
    }
}
