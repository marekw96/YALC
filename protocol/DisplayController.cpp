#include "DisplayController.hpp"

uint64_t DisplayController::ledsCount() const
{
    return 0;
}

YALC::ColorOrder DisplayController::colorOrder() const
{
    return YALC::ColorOrder::GRB;
}

void DisplayController::setColors(uint32_t startPixel, byte *data, uint32_t numberOfPixels)
{
}
