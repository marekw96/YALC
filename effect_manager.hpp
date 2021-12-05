#pragma once

#include "effect_information.hpp"

class effect_manager {
public:
    effect_manager();
    effect_information& get_current_effect();
    uint64_t get_number_of_effects();
    effect_information* get_effects();

private:
    effect_information* effects;
    int number_of_effects;
    effect_information* current_effect;
};