#include "brethe_leds.hpp"

effect_information brethe_leds::get_info()
{
    effect_information info;
    info.id = "brethe_leds";
    info.name = "Brethe";
    info.description = "Turn on and off all leds smoothly";
    info.init = std::bind(&brethe_leds::init, this, std::placeholders::_1);
    info.periodic = std::bind(&brethe_leds::periodic, this, std::placeholders::_1);
    info.deinit = std::bind(&brethe_leds::deinit, this);

    return info;
}

void brethe_leds::init(Adafruit_NeoPixel& pixels) {
    Serial.println("brethe_leds::init");

    this->pixels = &pixels;
    this->pixels->clear();

    this->counter = 0;
    this->color = 0;
    this->goes_up = true;
}

void brethe_leds::periodic(int64_t time_elapsed) {
    this->counter += time_elapsed;
    if(this->counter > 50000)
    {
        if(this->goes_up)
        {
            ++this->color;

            if(this->color == 255)
                this->goes_up = false;
        }
        else
        {
            --this->color;

            if(this->color == 0)
                this->goes_up = true;
        }

        for(int i = 0; i < 25; ++i)
            this->pixels->setPixelColor(i, this->pixels->Color(0, 0, this->color));

        this->pixels->show();
        this->counter = 0;
    }
}

void brethe_leds::deinit() {}