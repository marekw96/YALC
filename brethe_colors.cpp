#include "brethe_colors.hpp"

#include "effect_parameter.hpp"
#include "view.hpp"

static effect_parameter params[] = {
    {.name = "first color", .id="first_color", .default_value = "#ff00ff", .type = EFFECT_TYPE::COLOR},
    {.name = "second color", .id="second_color", .default_value = "#000000", .type = EFFECT_TYPE::COLOR},
};

effect_information brethe_colors::get_info()
{
    effect_information info;
    info.id = "brethe_colors";
    info.name = "Brethe colors";
    info.description = "Switch between colors smoothly";
    info.parameters = view<effect_parameter>(params, sizeof(params) / sizeof(params[0]));
    info.init = std::bind(&brethe_colors::init, this, std::placeholders::_1);
    info.periodic = std::bind(&brethe_colors::periodic, this, std::placeholders::_1);
    info.deinit = std::bind(&brethe_colors::deinit, this);
    info.set_parameter = std::bind(&brethe_colors::set_parameter, this, std::placeholders::_1, std::placeholders::_2);
    info.get_parameter = std::bind(&brethe_colors::get_parameter, this, std::placeholders::_1);

    return info;
}

void brethe_colors::init(Adafruit_NeoPixel& pixels) {
    Serial.println("brethe_colors::init");

    this->pixels = &pixels;
    this->pixels->clear();

    this->counter = 0;
    this->step_counter = 0;
    this->steps = 100;

    this->step.R = (this->second_color.R - this->first_color.R)/this->steps;
    this->step.G = (this->second_color.G - this->first_color.G)/this->steps;
    this->step.B = (this->second_color.B - this->first_color.B)/this->steps;

    for(int i = 0; i < 25; ++i)
        this->pixels->setPixelColor(i, this->pixels->Color(this->first_color.G, this->first_color.R, this->first_color.B));
}

void brethe_colors::periodic(int64_t time_elapsed) {
    this->counter += time_elapsed;

    if(this->counter > 50000)
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

            this->step.R = (this->second_color.R - this->first_color.R)/static_cast<float>(this->steps);
            this->step.G = (this->second_color.G - this->first_color.G)/static_cast<float>(this->steps);
            this->step.B = (this->second_color.B - this->first_color.B)/static_cast<float>(this->steps);
        }

        for(int i = 0; i < 25; ++i)
            this->pixels->setPixelColor(i, this->pixels->Color((uint8_t)current_color.G, (uint8_t)current_color.R, (uint8_t)current_color.B));

        this->pixels->show();
        this->counter = 0;
    }
}

bool brethe_colors::set_parameter(const String& name, const String& value) {
    if(name == "first_color")
    {
        if(!is_rgb_string_valid(value.c_str()))
            return false;

        this->first_color = to_rgb(value.c_str());

        return true;
    }
    else if(name == "second_color")
    {
        if(!is_rgb_string_valid(value.c_str()))
            return false;

        this->second_color = to_rgb(value.c_str());

        return true;
    }

    return false;
}

String brethe_colors::get_parameter(const String& name) {
    if(name == "first_color")
    {
        return to_hex_string(this->first_color).c_str();
    }
    else if(name == "second_color")
    {
        return to_hex_string(this->second_color).c_str();
    }

    return {};
}

void brethe_colors::deinit() {}