#include "blink_all_leds.hpp"

#include "effect_parameter.hpp"
#include "view.hpp"

static effect_parameter params[] = {
    {"Pierwszy kolor", "first_color", "#960096", EFFECT_TYPE::COLOR},
    {"Drugi kolor", "second_color", "#000000",  EFFECT_TYPE::COLOR},
    {"Szybkosc", "speed", "normalnie", EFFECT_TYPE::SELECT, "wolno;normalnie;szybko;"}
};

effect_information blink_all_leds::get_info()
{
    effect_information info;
    info.id = "blink_all_leds";
    info.name = "Mruganie";
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
    if(this->counter > this->timeout_ns)
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
    else if(name == "speed")
    {
        if(value == "szybko")
            this->timeout_ns = 250000;
        else if(value == "normalnie")
            this->timeout_ns = 500000;
        else if(value == "wolno")
            this->timeout_ns = 750000;
        else
            return false;

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
    else if(name == "speed") {
        if(this->timeout_ns == 250000)
            return "szybko";
        else if(this->timeout_ns == 500000)
            return "normalnie";
        else if(this->timeout_ns == 750000)
            return "wolno";
    }

    return {};
}

void blink_all_leds::deinit() {}