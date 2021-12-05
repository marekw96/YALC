#include "blink_all_leds.hpp"

effect_information blink_all_leds::get_info()
{
    effect_information info;
    info.id = "blink_all_leds";
    info.name = "Blink all leds";
    info.description = "Turn on and off all leds";
    info.init = std::bind(&blink_all_leds::init, this, std::placeholders::_1);
    info.periodic = std::bind(&blink_all_leds::periodic, this, std::placeholders::_1);
    info.deinit = std::bind(&blink_all_leds::deinit, this);

    return info;
}

void blink_all_leds::init(Adafruit_NeoPixel& pixels) {
    Serial.println("blink_all_leds::init");

    this->pixels = &pixels;
    this->pixels->clear();

    this->counter = 0;
    this->on = false;
}

void blink_all_leds::periodic(int64_t time_elapsed) {
    Serial.println(time_elapsed);
    Serial.println(this->counter);

    this->counter += time_elapsed;
    if(this->counter > 500000)
    {
        if(this->on)
        {
            this->pixels->clear();
            this->on = false;
        }
        else
        {
            for(int i = 0; i < 25; ++i)
                this->pixels->setPixelColor(i, this->pixels->Color(0, 150, 150));

            this->on = true;
        }

        this->pixels->show();
        this->counter = 0;
    }
}

void blink_all_leds::deinit() {}