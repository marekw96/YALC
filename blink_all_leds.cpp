#include "blink_all_leds.hpp"

#include "effect_parameter.hpp"
#include "view.hpp"

static effect_parameter params[] = {
    {"first color", "first_color", "#960096", EFFECT_TYPE::COLOR},
    {"second color", "second_color", "#000000",  EFFECT_TYPE::COLOR},
};

effect_information blink_all_leds::get_info()
{
    effect_information info;
    info.id = "blink_all_leds";
    info.name = "Blink all leds";
    info.description = "Turn on and off all leds";
    info.parameters = view<effect_parameter>(params, sizeof(params) / sizeof(params[0]));
    info.init = std::bind(&blink_all_leds::init, this, std::placeholders::_1);
    info.periodic = std::bind(&blink_all_leds::periodic, this, std::placeholders::_1);
    info.deinit = std::bind(&blink_all_leds::deinit, this);
    info.set_parameter = std::bind(&blink_all_leds::set_parameter, this, std::placeholders::_1, std::placeholders::_2);
    info.get_parameter = std::bind(&blink_all_leds::get_parameter, this, std::placeholders::_1);

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
    this->counter += time_elapsed;
    if(this->counter > 500000)
    {
        if(this->on)
        {
            for(int i = 0; i < this->pixels->numPixels(); ++i)
                this->pixels->setPixelColor(i, this->pixels->Color(this->second_color.G, this->second_color.R, this->second_color.B));
            this->on = false;
        }
        else
        {
            for(int i = 0; i < this->pixels->numPixels(); ++i)
                this->pixels->setPixelColor(i, this->pixels->Color(this->first_color.G, this->first_color.R, this->first_color.B));

            this->on = true;
        }

        this->pixels->show();
        this->counter = 0;
    }
}

bool blink_all_leds::set_parameter(const String& name, const String& value) {
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

String blink_all_leds::get_parameter(const String& name) {
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

void blink_all_leds::deinit() {}