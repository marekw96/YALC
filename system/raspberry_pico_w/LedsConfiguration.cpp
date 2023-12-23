#include "LedsConfiguration.hpp"

LedsConfiguration::LedsConfiguration(Application &app)
    :app(app), leds{{0, LedColorConfig::GRB}, {100, LedColorConfig::GRB}}
{
}

bool LedsConfiguration::init()
{
    auto cfg = app.storage->read_string("cfg/leds");

    if(cfg.empty()) {
        storeConfig();
    }
    {
        printf("[LedsConfiguration]init: %s\n", cfg.c_str());
        auto findNewLine = cfg.find("\n");

        auto line0 = cfg.substr(0, findNewLine);
        auto splitPoint = line0.find(",");
        if(splitPoint == std::string::npos){
            leds[0].length = std::stoi(line0);
        }
        else{
            auto count = line0.substr(0, splitPoint);
            auto config = line0.substr(splitPoint + 1);
            leds[0].length = std::stoi(line0);
            if(config == "GRB")
                leds[0].colorConfig = LedColorConfig::GRB;
            else if(config == "BGR")
                leds[0].colorConfig = LedColorConfig::BGR;
            else
                leds[0].colorConfig = LedColorConfig::RGB;
        }

        printf("[LedsConfiguration] 0 -> %d, %s\n", leds[0].length, to_char(leds[0].colorConfig));

        auto line1 = cfg.substr(findNewLine + 1);
        splitPoint = line1.find(",");
        if(splitPoint == std::string::npos){
            leds[1].length = std::stoi(line1);
        }
        else{
            auto count = line1.substr(0, splitPoint);
            auto config = line1.substr(splitPoint + 1);
            leds[1].length = std::stoi(line1);
            if(config == "GRB")
                leds[1].colorConfig = LedColorConfig::GRB;
            else if(config == "BGR")
                leds[1].colorConfig = LedColorConfig::BGR;
            else
                leds[1].colorConfig = LedColorConfig::RGB;
        }
        printf("[LedsConfiguration] 1 -> %d, %s\n", leds[1].length, to_char(leds[1].colorConfig));
    }

    return true;
}

uint32_t LedsConfiguration::getPixelsFor(uint32_t pin)
{
    //printf("[LedsConfiguration]getPixelsFor %d: %d\n", pin, leds[pin].length);
    return leds[pin].length;
}

void LedsConfiguration::setPixelsFor(uint32_t pin, uint32_t value)
{
    leds[pin].length = value;
    printf("[LedsConfiguration]setPixelsFor %d %d\n", pin, leds[pin].length);
    storeConfig();
}

void LedsConfiguration::setColorConfigFor(uint32_t pin, LedColorConfig value)
{
    leds[pin].colorConfig = value;
    printf("[LedsConfiguration]setColorConfigFor %d %s\n", pin, to_char(leds[pin].colorConfig));
    storeConfig();
}

uint32_t LedsConfiguration::getNumberOfPins() const
{
    return 2;
}

LedColorConfig LedsConfiguration::getColorConfig(uint32_t pin)
{
    return leds[pin].colorConfig;
}

void LedsConfiguration::storeConfig()
{
    std::string txt = std::to_string(leds[0].length) + std::string(",") + std::string(to_char(leds[0].colorConfig)) + "\n"
                      + std::to_string(leds[1].length) + std::string(",") + std::string(to_char(leds[1].colorConfig)) ;
    app.storage->store("cfg/leds", txt);
}

const char * to_char(LedColorConfig val)
{
    switch(val){
        case LedColorConfig::GRB:
            return "GRB";
        case LedColorConfig::RGB:
            return "RGB";
        case LedColorConfig::BGR:
            return "BGR";
        default:
            return "?";
    }
}

YALC::ColorOrder to_colorOrder(LedColorConfig val)
{
    switch(val){
        case LedColorConfig::GRB:
            return YALC::ColorOrder::GRB;
        case LedColorConfig::RGB:
            return YALC::ColorOrder::RGB;
        case LedColorConfig::BGR:
            return YALC::ColorOrder::BGR;
        default:
            return YALC::ColorOrder::GRB;
    }
}
