#include "effect_manager.hpp"

#include "blink_all_leds.hpp"
#include "brethe_colors.hpp"
#include "random_leds.hpp"
#include "off_leds.hpp"
#include "filling_leds.hpp"

blink_all_leds blink_all_leds_obj;
brethe_colors brethe_colors_obj;
random_leds random_leds_obj;
off_leds off_leds_obj;
filling_leds filling_leds_obj;

effect_information infos[5];

effect_manager::effect_manager()
{
    infos[0] = off_leds_obj.get_info();
    infos[1] = blink_all_leds_obj.get_info();
    infos[2] = brethe_colors_obj.get_info();
    infos[3] = random_leds_obj.get_info();
    infos[4] = filling_leds_obj.get_info();

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

            this->cfg_manager->store_value("config", "last_used", this->current_effect->id);

            return true;
        }
    }

    return false;
}

void effect_manager::set_pixels(Adafruit_NeoPixel& pixels) {
    this->pixels = &pixels;
}

void effect_manager::set_config_manager(config_manager& cfg_manager){
    this->cfg_manager = &cfg_manager;
    this->change_effect_to(this->cfg_manager->read_value("config", "last_used").c_str());
}

view<effect_parameter> effect_manager::get_effect_parameters(const char* effect_id)
{
    String id = effect_id;
    for(int i = 0; i < this->number_of_effects; ++i)
    {
        if(id == this->effects[i].id)
        {
            return this->effects[i].parameters;
        }
    }

    return {};
}

bool effect_manager::effect_with_id_exists(const String& effect_id) {
    for(int i = 0; i < this->number_of_effects; ++i)
    {
        if(effect_id == this->effects[i].id)
        {
            return true;
        }
    }

    return false;
}

bool effect_manager::set_effect_parameter(const String& name, const String& value)
{
    if(this->current_effect->set_parameter){
        if(this->current_effect->set_parameter(name, value)) {
            this->cfg_manager->store_value(this->current_effect->id, name.c_str(), value.c_str());
            return true;
        }
    }

    return false;
}

String effect_manager::get_effect_parameter(const String& name)
{
    if(this->current_effect->get_parameter)
    {
        return this->current_effect->get_parameter(name);
    }

    return "";
}

void effect_manager::restore_effect_parameters() {
    for(int effect_idx = 0; effect_idx < this->number_of_effects; ++effect_idx) {
        auto& effect = this->effects[effect_idx];

        for(int param_idx = 0; param_idx < effect.parameters.size(); ++param_idx) {
            auto& parameter = effect.parameters[param_idx];

            auto value = this->cfg_manager->read_value(effect.id, parameter.id);
            if(value.isEmpty())
            {
                value = parameter.default_value;
            }

            effect.set_parameter(parameter.id, value);
        }
    }
}