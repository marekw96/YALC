#pragma once

enum class EFFECT_TYPE : uint8_t {
    COLOR = 0
};

struct effect_parameter {
    const char* name;
    const char* id;
    const char* default_value;
    EFFECT_TYPE type;
};