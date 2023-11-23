#pragma once

#include "Storage.hpp"

class InternetManager;
class EffectsManager;
class Storage;

struct Application {
    InternetManager* internetManager;
    EffectsManager* effectsManager;
    Storage* storage;
};