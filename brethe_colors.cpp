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
    this->second_color = {255, 0, 0}; //RED
    this->first_color = {0, 255, 0}; //GREEN
    this->steps = 25;

    this->step.R = (this->second_color.R - this->first_color.R)/this->steps;
    this->step.G = (this->second_color.G - this->first_color.G)/this->steps;
    this->step.B = (this->second_color.B - this->first_color.B)/this->steps;

    for(int i = 0; i < 25; ++i)
        this->pixels->setPixelColor(i, this->pixels->Color(this->first_color.G, this->first_color.R, this->first_color.B));
}

void brethe_colors::periodic(int64_t time_elapsed) {
    this->counter += time_elapsed;

    if(this->counter > 200000)// 0.2 sec
    {
        ++this->step_counter;

        RGB_t<float> current_color;
        current_color.R = this->step_counter * this->step.R;
        current_color.G = this->step_counter * this->step.G;
        current_color.B = this->step_counter * this->step.B;

        if(this->step_counter == this->steps)
        {
            auto temp = this->second_color;
            this->second_color = this->first_color;
            this->first_color = temp;
            this->step_counter = 0;

            this->step.R = (this->second_color.R - this->first_color.R)/this->steps;
            this->step.G = (this->second_color.G - this->first_color.G)/this->steps;
            this->step.B = (this->second_color.B - this->first_color.B)/this->steps;
        }

        for(int i = 0; i < 25; ++i)
            this->pixels->setPixelColor(i, this->pixels->Color((uint8_t)current_color.G, (uint8_t)current_color.R, (uint8_t)current_color.B));

        this->pixels->show();
        this->counter = 0;
    }
}

void brethe_colors::deinit() {}