#pragma once
#include <string>

enum class ParameterType {
    Color = 'c'
};

struct ParameterDescription {
    ParameterType type;
    std::string name;
    std::string defaultValue;
    std::string value;
};

enum class InputType {
    INVALID = 0,
    DIGITAL,
    ANALOG,
    RAW
};

struct InputDescription {
    InputType type;
    std::string name;
};

