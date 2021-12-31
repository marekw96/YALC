#include "sloping_leds.hpp"

#include "effect_parameter.hpp"
#include "view.hpp"

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

static effect_parameter params[] = {
    {"Kolor", "color", "#FFFFFF", EFFECT_TYPE::COLOR},
    {"Szybkosc", "speed",  "normal", EFFECT_TYPE::SELECT, "bardzo_wolno;wolniej;wolno;normalnie;szybko;"},
    {"Zmienne kolory", "change_colors", "on", EFFECT_TYPE::SELECT, "on;off;"}
};

RGB predefined_colors[] = {
    {0xFF, 0xFF, 0xFF},
    {0xFF, 0, 0},
    {0, 0, 0xFF},
    {0, 0xFF, 0},
    {0xFF, 0xFF, 0},
    {0xFF, 0, 0xFF},
    {0, 0xFF, 0xFF}
};


effect_information sloping_leds::get_info()
{
    effect_information info;
    info.id = "sloping_leds";
    info.name = "Opadanie";
    info.description = "";
    info.parameters = view<effect_parameter>(params, sizeof(params) / sizeof(params[0]));
    info.init = std::bind(&sloping_leds::init, this, std::placeholders::_1);
    info.periodic = std::bind(&sloping_leds::periodic, this, std::placeholders::_1);
    info.deinit = std::bind(&sloping_leds::deinit, this);
    info.set_parameter = std::bind(&sloping_leds::set_parameter, this, std::placeholders::_1, std::placeholders::_2);
    info.get_parameter = std::bind(&sloping_leds::get_parameter, this, std::placeholders::_1);

    return info;
}

void sloping_leds::init(Adafruit_NeoPixel& pixels) {
    Serial.println("sloping_leds::init");

    if(this->slopes)
        this->deinit();

    this->pixels = &pixels;
    this->pixels->clear();
    this->pixels->show();

    this->counter = 0;
    this->position = this->pixels->numPixels() -1;

    this->num_of_slopes = this->pixels->numPixels()/10;
    this->slopes = new slope[this->num_of_slopes];
    this->last_used_color = 0;

    for(int i = 0; i < this->num_of_slopes; ++i) {
        auto& slope = this->slopes[i];

        if(this->change_colors)
            slope.color = predefined_colors[(this->last_used_color++)%ARRAY_SIZE(predefined_colors)];
        else
            slope.color = this->color;

        slope.position = this->pixels->numPixels() + i * (this->pixels->numPixels() / this->num_of_slopes);
    }
}

void sloping_leds::periodic(int64_t time_elapsed) {
    auto put_pixels = [this](int position, const RGB& color) {
        if(position < 0 || position >= this->pixels->numPixels())
            return;

        this->pixels->setPixelColor(position, this->pixels->Color(color.G, color.R, color.B));
    };

    auto color_fade = [](const RGB& color, float multiplier) {
        return RGB{color.R * multiplier, color.G * multiplier, color.B * multiplier};
    };

    this->counter += time_elapsed;
    if(this->counter > this->timeout_ns)
    {
        this->pixels->clear();

        for(int i = 0; i < this->num_of_slopes; ++i) {
            auto& slope = this->slopes[i];

            put_pixels(slope.position, slope.color);
            put_pixels(slope.position+1, color_fade(slope.color, 0.75f));
            put_pixels(slope.position+2, color_fade(slope.color, 0.55f));
            put_pixels(slope.position+3, color_fade(slope.color, 0.35f));
            put_pixels(slope.position+4, color_fade(slope.color, 0.1f));

            --slope.position;

            if(slope.position <= -5) {
                slope.position = this->pixels->numPixels();

                if(this->change_colors)
                    slope.color = predefined_colors[(this->last_used_color++)%ARRAY_SIZE(predefined_colors)];
            }
        }

        this->pixels->show();
        this->counter = 0;
    }
}

bool sloping_leds::set_parameter(const String& name, const String& value) {
    if(name == "color")
    {
        if(!is_rgb_string_valid(value.c_str()))
            return false;

        this->color = to_rgb(value.c_str());

        return true;
    }
    else if(name == "speed")
    {
        if(value == "szybko")
            this->timeout_ns = 30000;
        else if(value == "normalnie")
            this->timeout_ns = 60000;
        else if(value == "wolno")
            this->timeout_ns = 100000;
        else if(value == "wolniej")
            this->timeout_ns = 200000;
        else if(value == "bardzo_wolno")
            this->timeout_ns = 300000;
        else
            return false;

        return true;
    }
    else if(name == "change_colors")
    {
        if(value == "on")
            this->change_colors = true;
        else if(value == "off")
            this->change_colors = false;
        else
            return false;

        return true;
    }

    return false;
}

String sloping_leds::get_parameter(const String& name) {
    if(name == "color")
    {
        return to_hex_string(this->color).c_str();
    }
    else if(name == "speed") {
        if(this->timeout_ns == 30000)
            return "szybko";
        else if(this->timeout_ns == 60000)
            return "normalnie";
        else if(this->timeout_ns == 100000)
            return "wolno";
        else if(this->timeout_ns == 200000)
            return "wolniej";
        else if(this->timeout_ns == 300000)
            return "bardzo_wolno";
        else
            return "";
    }
    else if(name == "change_colors") {
        if(this->change_colors)
            return "on";
        else
            return "off";
    }

    return {};
}

void sloping_leds::deinit() {
    if(this->slopes) {
        delete[] this->slopes;

        this->slopes = nullptr;
    }
}