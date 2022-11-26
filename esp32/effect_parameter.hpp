#pragma once

enum class EFFECT_TYPE : uint8_t {
    COLOR = 0,
    SELECT,
};

struct effect_parameter {
    effect_parameter(const char* name, const char* id, const char* default_value, EFFECT_TYPE type)
        : name(name), id(id), default_value(default_value), type(type) {}

    effect_parameter(const char* name, const char* id, const char* default_value, EFFECT_TYPE type, const char* values)
        : name(name), id(id), default_value(default_value), type(type), values(values) {}
    const char* name;
    const char* id;
    const char* default_value;
    EFFECT_TYPE type;
    const char* values = nullptr;
};