#pragma once

#include <string>
#include <stdint.h>

template <typename T>
struct RGB_t {
    T R;
    T G;
    T B;
};

using RGB = RGB_t<uint8_t>;

std::string to_hex_string(const RGB& rgb);
bool is_rgb_string_valid(const std::string& input);
RGB to_rgb(const std::string& input);