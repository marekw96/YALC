#include "random_leds.hpp"

effect_information random_leds::get_info()
{
    effect_information info;
    info.id = "random_leds";
    info.name = "Random";
    info.description = "All is random";
    info.init = std::bind(&random_leds::init, this, std::placeholders::_1);
    info.periodic = std::bind(&random_leds::periodic, this, std::placeholders::_1);
    info.deinit = std::bind(&random_leds::deinit, this);

    return info;
}

void random_leds::init(Adafruit_NeoPixel& pixels) {
    static uint32_t init_random = esp_random();

    Serial.println("random_leds::init");

    this->pixels = &pixels;
    this->pixels->clear();

    this->counter = 0;

    for(int i = 0; i < 25; ++i)
        this->pixels->setPixelColor(i, this->pixels->Color(random(0, 255), random(0, 255), random(0, 255)));
}

void random_leds::periodic(int64_t time_elapsed) {
    this->counter += time_elapsed;
    int steps = 25;
    if(this->counter > 200000)
    {
        this->pixels->setPixelColor(random(0, 25), this->pixels->Color(random(0, 255), random(0, 255), random(0, 255)));

        this->pixels->show();
        this->counter = 0;
    }
}

void random_leds::deinit() {}