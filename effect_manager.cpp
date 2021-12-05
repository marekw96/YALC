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