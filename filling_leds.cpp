#include "filling_leds.hpp"

static effect_parameter params[] = {
    {.name = "Pierwszy kolor", .id="first_color", .default_value = "#ff00ff", .type = EFFECT_TYPE::COLOR},
    {.name = "sDrugi kolor", .id="second_color", .default_value = "#000000", .type = EFFECT_TYPE::COLOR},
    {"Szybkosc", "speed",  "normalnie", EFFECT_TYPE::SELECT, "wolno;normalnie;szybko;"}
};

effect_information filling_leds::get_info()
{
    effect_information info;
    info.id = "filling_leds";
    info.name = "Wypelnianie";
    info.description = "Filling from one end";
    info.parameters = view<effect_parameter>(params, sizeof(params) / sizeof(params[0]));
    info.init = std::bind(&filling_leds::init, this, std::placeholders::_1);
    info.periodic = std::bind(&filling_leds::periodic, this, std::placeholders::_1);
    info.deinit = std::bind(&filling_leds::deinit, this);
    info.set_parameter = std::bind(&filling_leds::set_parameter, this, std::placeholders::_1, std::placeholders::_2);
    info.get_parameter = std::bind(&filling_leds::get_parameter, this, std::placeholders::_1);

    return info;
}

void filling_leds::init(Adafruit_NeoPixel& pixels) {
    Serial.println("filling_leds::init");

    this->pixels = &pixels;
    this->pixels->clear();

    this->current_position = this->pixels->numPixels() - 1;
    this->filled = 0;

    for(int i = 0; i < this->pixels->numPixels(); ++i)
        this->pixels->setPixelColor(i, this->pixels->Color(this->first_color.G, this->first_color.R, this->first_color.B));

    this->pixels->show();
    this->counter = 0;
}

void filling_leds::periodic(int64_t time_elapsed) {
    this->counter += time_elapsed;
    if(this->counter > this->timeout_ns)
    {
        if(this->current_position != this->pixels->numPixels() - 1)
        {
            this->pixels->setPixelColor(this->current_position+1, this->pixels->Color(this->first_color.G, this->first_color.R, this->first_color.B));
        }
        this->pixels->setPixelColor(this->current_position, this->pixels->Color(this->second_color.G, this->second_color.R, this->second_color.B));

        --this->current_position;

        if(this->current_position == (this->filled-1))
        {
            ++this->filled;
            this->current_position = this->pixels->numPixels() - 1;
        }

        if(this->filled == this->pixels->numPixels() -1)
        {
            auto tmp = this->first_color;
            this->first_color = this->second_color;
            this->second_color = tmp;

            this->filled = 0;
            this->current_position = this->pixels->numPixels() - 1;
        }

        this->pixels->show();
        this->counter = 0;
    }
}

bool filling_leds::set_parameter(const String& name, const String& value) {
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
    else if(name == "speed") {
        if(value == "szybko")
            this->timeout_ns = 100000;
        else if(value == "normalnie")
            this->timeout_ns = 300000;
        else if(value == "wolno")
            this->timeout_ns = 600000;
        else
            return false;

        return true;
    }

    return false;
}

String filling_leds::get_parameter(const String& name) {
    if(name == "first_color")
    {
        return to_hex_string(this->first_color).c_str();
    }
    else if(name == "second_color")
    {
        return to_hex_string(this->second_color).c_str();
    }
    else if(name == "speed") {
        if(this->timeout_ns == 100000)
            return "szybko";
        else if(this->timeout_ns == 300000)
            return "normalnie";
        else if(this->timeout_ns == 600000)
            return "wolno";
        else
            return "";
    }

    return {};
}

void filling_leds::deinit() {}