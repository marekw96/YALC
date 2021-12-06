#include "effect_manager.hpp"

#include "blink_all_leds.hpp"
#include "brethe_leds.hpp"

blink_all_leds blink_all_leds_obj;
brethe_leds brethe_leds_obj;

effect_information infos[2];

effect_manager::effect_manager()
{
    infos[0] = blink_all_leds_obj.get_info();
    infos[1] = brethe_leds_obj.get_info();

    this->current_effect = &infos[0];
    this->number_of_effects = sizeof(infos) / sizeof(infos[0]);
    this->effects = &infos[0];
}

effect_information& effect_manager::get_current_effect() {
    return *this->current_effect;
}

uint64_t effect_manager::get_number_of_effects() {
    return this->number_of_effects;
}

effect_information* effect_manager::get_effects() {
    return this->effects;
}

bool effect_manager::change_effect_to(const char* effect_id) {
    String id = effect_id;
    for(int i = 0; i < this->number_of_effects; ++i)
    {
        if(id == this->effects[i].id)
        {
            this->current_effect->deinit();

            this->current_effect = &this->effects[i];
            this->current_effect->init(*this->pixels);

            return true;
        }
    }

    return false;
}

void effect_manager::set_pixels(Adafruit_NeoPixel& pixels)
{
    this->pixels = &pixels;
}