#pragma once

#include "Storage.hpp"


class InternetManager;
class EffectsManager;
class Storage;
class LedsConfiguration;

#include "../pythonAnimations/AnimationEngine.hpp"
#include "ws2812/LedString_ws2812.hpp"
#include "ws2812/LedStringCombined.hpp"

struct Application {
    InternetManager* internetManager;
    EffectsManager* effectsManager;
    Storage* storage;
    LedsConfiguration* ledsConfiguration;
    AnimationEngine<LedStringCombined<LedString_ws2812,LedString_ws2812>>* animationEngine;

    bool shouldBeRebooted = false;
};