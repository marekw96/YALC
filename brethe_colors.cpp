#include "brethe_colors.hpp"

effect_information brethe_colors::get_info()
{
    effect_information info;
    info.id = "brethe_colors";
    info.name = "Brethe colors";
    info.description = "Switch between colors smoothly";
    info.init = std::bind(&brethe_colors::init, this, std::placeholders::_1);
    info.periodic = std::bind(&brethe_colors::periodic, this, std::placeholders::_1);
    info.deinit = std::bind(&brethe_colors::deinit, this);

    return info;
}

void brethe_colors::init(Adafruit_NeoPixel& pixels) {
    Serial.println("brethe_colors::init");

    this->pixels = &pixels;
    this->pixels->clear();

    this->counter = 0;
    this->step_counter = 0;
    this->first_color = {255, 0, 0}; //RED
    this->second_color = {0, 255, 0}; //GREEN
    this->current_color_R = this->first_color.R;
    this->current_color_G = this->first_color.G;
    this->current_color_B = this->first_color.B;

    for(int i = 0; i < 25; ++i)
        this->pixels->setPixelColor(i, this->pixels->Color((uint8_t)this->current_color_G, (uint8_t)this->current_color_R, (uint8_t)this->current_color_B));
}

void brethe_colors::periodic(int64_t time_elapsed) {
    this->counter += time_elapsed;
    int steps = 25;
    if(this->counter > 200000)
    {
        ++this->step_counter;

        RGB step = {0,0,0};
        float step_R, step_G, step_B;
        step_R = (this->second_color.R - this->first_color.R)/steps;
        step_G = (this->second_color.G - this->first_color.G)/steps;
        step_B = (this->second_color.B - this->first_color.B)/steps;

        this->current_color_R += step_R;
        this->current_color_G += step_G;
        this->current_color_B += step_B;

        if(this->step_counter == steps)
        {
            auto temp = this->second_color;
            this->second_color = this->first_color;
            this->first_color = temp;
            this->step_counter = 0;
        }

        for(int i = 0; i < 25; ++i)
            this->pixels->setPixelColor(i, this->pixels->Color((uint8_t)this->current_color_G, (uint8_t)this->current_color_R, (uint8_t)this->current_color_B));

        this->pixels->show();
        this->counter = 0;
    }
}

void brethe_colors::deinit() {}