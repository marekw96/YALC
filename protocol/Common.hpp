#pragma once

#include <stdint.h>

using byte = unsigned char;
// using uint16_t = unsigned short;
// using uint32_t = unsigned int;
// using uint64_t = unsigned long long;

namespace YALC {
    enum class ColorOrder {RGB=0,
                           GRB=1};

    struct PixelData8Bit { byte data[3] = {0}; };
}