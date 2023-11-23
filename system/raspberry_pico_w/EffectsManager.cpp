#include "EffectsManager.hpp"

#include "../pythonAnimations/MyAnimation.py.hpp"
#include "../pythonAnimations/FallingStar.py.hpp"

EffectsManager::EffectsManager(Application &app)
    :app(app)
{
}

std::vector<EffectDescription> EffectsManager::getListOfEffects()
{
    return {{0,"MyAnimation"}, {1,"FallingStar"}};
}

std::string EffectsManager::getEffectCode(uint32_t id)
{
    if(id == 0) {
        return MyAnimation_py;
    }
    else if(id == 1) {
        return FallingStar_py;
    }

    return "";
}

bool EffectsManager::selectEffect(uint32_t newId)
{
    selectedEffect = newId;
    effectChanged = true;
    printf("[EffectsManager]Changed effect to %d\n", newId);
    return true;
}

uint32_t EffectsManager::getSelectedEffectId()
{
    return selectedEffect;
}

bool EffectsManager::hasEffectChanged()
{
    return effectChanged;
}

void EffectsManager::restartedEffects()
{
    effectChanged = false;
}
