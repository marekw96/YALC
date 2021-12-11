#pragma once

template <typename T>
struct RGB_t {
    T R;
    T G;
    T B;
};

using RGB = RGB_t<uint8_t>;