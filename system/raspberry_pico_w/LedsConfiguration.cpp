#include "LedsConfiguration.hpp"

LedsConfiguration::LedsConfiguration(Application &app)
    :app(app), leds{0,100}
{
}

bool LedsConfiguration::init()
{
    auto cfg = app.storage->read_string("cfg/leds");

    if(cfg.empty()) {
        storeConfig();
    }
    {
        auto findNewLine = cfg.find("\n");
        auto leds0 = std::stoi(cfg.substr(0, findNewLine));
        auto leds1 = std::stoi(cfg.substr(findNewLine + 1));

        leds[0] = leds0;
        leds[1] = leds1;
    }

    return true;
}

uint32_t LedsConfiguration::getPixelsFor(uint32_t pin)
{
    printf("[LedsConfiguration]getPixelsFor %d: %d\n", pin, leds[pin]);
    return leds[pin];
}

void LedsConfiguration::setPixelsFor(uint32_t pin, uint32_t value)
{
    leds[pin] = value;
    printf("[LedsConfiguration]setPixelsFor %d %d -> %d\n", pin, value, leds[pin]);
    storeConfig();
}

uint32_t LedsConfiguration::getNumberOfPins() const
{
    return 2;
}

void LedsConfiguration::storeConfig()
{
    std::string txt = std::to_string(leds[0]) + "\n" + std::to_string(leds[1]);
    app.storage->store("cfg/leds", txt);
}
