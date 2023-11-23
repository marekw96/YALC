#pragma once

#include "Application.hpp"

#include <string>
#include <vector>

struct EffectDescription {
    uint32_t id;
    std::string name;
};

class EffectsManager {
public:
    EffectsManager(Application& app);
    std::vector<EffectDescription> getListOfEffects();
    std::string getEffectCode(uint32_t id);
    bool selectEffect(uint32_t newId);
    uint32_t getSelectedEffectId();
private:
    Application& app;
    uint32_t selectedEffect = 0;
};