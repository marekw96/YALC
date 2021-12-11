#include "filling_leds.hpp"

effect_information filling_leds::get_info()
{
    effect_information info;
    info.id = "filling_leds";
    info.name = "Filling";
    info.description = "Filling from one end";
    info.init = std::bind(&filling_leds::init, this, std::placeholders::_1);
    info.periodic = std::bind(&filling_leds::periodic, this, std::placeholders::_1);
    info.deinit = std::bind(&filling_leds::deinit, this);

    return info;
}

void filling_leds::init(Adafruit_NeoPixel& pixels) {
    Serial.println("filling_leds::init");

    this->pixels = &pixels;
    this->pixels->clear();

    this->first_color = {255,255,255};
    this->second_color = {255, 0, 0};
    this->current_position = 24;
    this->filled = 0;

    for(int i = 0; i < 25; ++i)
        this->pixels->setPixelColor(i, this->pixels->Color(this->first_color.G, this->first_color.R, this->first_color.B));

    this->pixels->show();
    this->counter = 0;
}

void filling_leds::periodic(int64_t time_elapsed) {
    this->counter += time_elapsed;
    if(this->counter > 300000) //0.3s
    {
        if(this->current_position != 24)
        {
            this->pixels->setPixelColor(this->current_position+1, this->pixels->Color(this->first_color.G, this->first_color.R, this->first_color.B));
        }
        this->pixels->setPixelColor(this->current_position, this->pixels->Color(this->second_color.G, this->second_color.R, this->second_color.B));

        --this->current_position;

        if(this->current_position == (this->filled-1))
        {
            ++this->filled;
            this->current_position = 24;
        }

        if(this->filled == 24)
        {
            auto tmp = this->first_color;
            this->first_color = this->second_color;
            this->second_color = tmp;

            this->filled = 0;
            this->current_position = 24;
        }

        this->pixels->show();
        this->counter = 0;
    }
}

void filling_leds::deinit() {}