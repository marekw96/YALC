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

