#pragma once
#include <Adafruit_NeoPixel.h>

#include "effect_information.hpp"
#include "effect_parameter.hpp"
#include "view.hpp"
#include "config_manager.hpp"

class effect_manager {
public:
    effect_manager();
    effect_information& get_current_effect();
    uint64_t get_number_of_effects();
    effect_information* get_effects();
    void set_pixels(Adafruit_NeoPixel& pixels);
    void set_config_manager(config_manager& cfg_manager);
    bool change_effect_to(const char* effect_id);
    view<effect_parameter> get_effect_parameters(const char* effect_id);
    bool set_effect_parameter(const String& name, const String& value);
    bool effect_with_id_exists(const String& effect_id);
    String get_effect_parameter(const String& name);
    void restore_effect_parameters();

private:
    effect_information* effects;
    int number_of_effects;
    effect_information* current_effect;
    Adafruit_NeoPixel* pixels;
    config_manager* cfg_manager;
};