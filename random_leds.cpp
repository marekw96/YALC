#include "random_leds.hpp"
#include "rgb.hpp"

RGB colors[] = {
    {255, 0, 0}, // red
    {0, 255, 0}, // green
    {0, 0, 255}, // blue
    {255, 255, 255}, // white
    {255, 255, 0},
    {255, 0, 255},
    {0, 255, 255},
};

uint8_t num_colors = sizeof(colors) / sizeof(colors[0]);

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

    for(int i = 0; i < this->pixels->numPixels(); ++i)
    {
        auto& color = colors[random(0, num_colors)];
        this->pixels->setPixelColor(i, this->pixels->Color(color.G, color.R, color.B));
    }
}

void random_leds::periodic(int64_t time_elapsed) {
    this->counter += time_elapsed;

    if(this->counter > 200000)
    {
        auto& color = colors[random(0, num_colors)];
        this->pixels->setPixelColor(random(0, this->pixels->numPixels()), this->pixels->Color(color.G, color.R, color.B));

        this->pixels->show();
        this->counter = 0;
    }
}

void random_leds::deinit() {}