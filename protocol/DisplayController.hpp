#pragma once

#include "Common.hpp"

class DisplayController {
public:
    uint64_t ledsCount() const;
    YALC::ColorOrder colorOrder() const;
    void setColors(uint32_t startPixel, byte* data, uint32_t numberOfPixels);
};