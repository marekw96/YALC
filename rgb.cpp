#include "rgb.hpp"

#include <sstream>
#include <iomanip>
#include <stdlib.h>

std::string to_hex_string(const RGB& rgb) {
    std::stringstream stream;

    stream << "#";
    stream << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(rgb.R);
    stream << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(rgb.G);
    stream << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(rgb.B);

    return stream.str();
}

bool is_rgb_string_valid(const std::string& input) {
    return input[0] == '#' && input.length() == 7;
}

RGB to_rgb(const std::string& input) {
    RGB rgb;
    rgb.R = strtoul(input.substr(1,2).c_str(), NULL, 16);
    rgb.G = strtoul(input.substr(3,2).c_str(), NULL, 16);
    rgb.B = strtoul(input.substr(5,2).c_str(), NULL, 16);

    return rgb;
}