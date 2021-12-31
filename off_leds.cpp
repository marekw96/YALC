#include "off_leds.hpp"

effect_information off_leds::get_info()
{
    effect_information info;
    info.id = "off_leds";
    info.name = "Wylacz ledy";
    info.description = "Turn off all leds";
    info.init = std::bind(&off_leds::init, this, std::placeholders::_1);
    info.periodic = std::bind(&off_leds::periodic, this, std::placeholders::_1);
    info.deinit = std::bind(&off_leds::deinit, this);

    return info;
}

void off_leds::init(Adafruit_NeoPixel& pixels) {
    Serial.println("off_leds::init");

    pixels.clear();
    pixels.show();
}

void off_leds::periodic(int64_t) {
}

void off_leds::deinit() {}