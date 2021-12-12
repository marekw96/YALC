#include "blink_all_leds.hpp"

#include "effect_parameter.hpp"
#include "view.hpp"

effect_parameter params[] = {
    {.name = "first color", .id="first_color", .default_value = "#960096", .type = EFFECT_TYPE::COLOR},
    {.name = "second color", .id="second_color", .default_value = "#000000", .type = EFFECT_TYPE::COLOR},
};

blink_all_leds::blink_all_leds() {
    for(int i = 0; i < sizeof(params)/sizeof(params[0]); ++i)
    {
        this->set_parameter(params[i].id, params[i].default_value);
    }
}

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
            for(int i = 0; i < 25; ++i)
                this->pixels->setPixelColor(i, this->pixels->Color(this->second_color.G, this->second_color.R, this->second_color.B));
            this->on = false;
        }
        else
        {
            for(int i = 0; i < 25; ++i)
                this->pixels->setPixelColor(i, this->pixels->Color(this->first_color.G, this->first_color.R, this->first_color.B));

            this->on = true;
        }

        this->pixels->show();
        this->counter = 0;
    }
}

bool blink_all_leds::set_parameter(const String& name, const String& value) {
    Serial.print(name);
    Serial.print(": ");
    Serial.println(value);

    if(name == "first_color")
    {
        if(value[0] != '#' || value.length() != 7)
            return false;

        this->first_color.R = strtol(value.substring(1,3).c_str(), NULL, 16);
        this->first_color.G = strtol(value.substring(3,5).c_str(), NULL, 16);
        this->first_color.B = strtol(value.substring(5,7).c_str(), NULL, 16);

        return true;
    }
    else if(name == "second_color")
    {
        if(value[0] != '#' || value.length() != 7)
            return false;

        this->second_color.R = strtol(value.substring(1,3).c_str(), NULL, 16);
        this->second_color.G = strtol(value.substring(3,5).c_str(), NULL, 16);
        this->second_color.B = strtol(value.substring(5,7).c_str(), NULL, 16);

        return true;
    }

    return false;
}

void blink_all_leds::deinit() {}