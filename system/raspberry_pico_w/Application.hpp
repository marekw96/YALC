#pragma once

#include "Storage.hpp"

class InternetManager;
class EffectsManager;
class Storage;

#include "../pythonAnimations/AnimationEngine.hpp"
#include "ws2812/LedString_ws2812.hpp"

struct Application {
    InternetManager* internetManager;
    EffectsManager* effectsManager;
    Storage* storage;
    AnimationEngine<LedString_ws2812>* animationEngine;
};