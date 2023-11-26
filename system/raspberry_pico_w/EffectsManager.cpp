#include "EffectsManager.hpp"

#include "../pythonAnimations/MyAnimation.py.hpp"
#include "../pythonAnimations/FallingStar.py.hpp"

EffectsManager::EffectsManager(Application &app)
    :app(app)
{
}

bool EffectsManager::init()
{
    auto readout = app.storage->read_uint32_t("cfg/ef_last_reg");
    if(readout != 0)
        lastRegisteredId = readout;

    printf("[EffectsManager]lastRegisteredId: %d\n", lastRegisteredId);

    return true;
}

std::vector<EffectDescription> EffectsManager::getListOfEffects()
{
    return effects;
}

std::string EffectsManager::getEffectCode(uint32_t id)
{
    if(id == 0) {
        return MyAnimation_py;
    }
    else if(id == 1) {
        return FallingStar_py;
    }
    else {
        printf("[EffectsManager]fetching code from flash\n");
        auto code = fetchEffectCode(id);
        return code;
    }

    return "";
}

bool EffectsManager::selectEffect(uint32_t newId)
{
    if(selectedEffect != newId) {
        selectedEffect = newId;
        effectChanged = true;
        printf("[EffectsManager]Changed effect to %d\n", newId);
        app.storage->store("cfg/eff_selected", selectedEffect);
    }
    return effectChanged;
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

bool EffectsManager::addNewEffect(const std::string &name, const std::string &code)
{
    auto id = lastRegisteredId + 1;
    printf("Adding new effect with id %d\n", id);
    app.storage->makeDir("eff");

    auto path = std::string("eff/e_") + std::to_string(id);
    if(!app.storage->makeDir(path.c_str())){
        printf("[EffectsManager] failed to create directory for effect. but moving on...\n");
        //return false;
    }

    auto pathCode = path + "/code";
    if(!app.storage->store(pathCode.c_str(), code)) {
        printf("[EffectsManager] failed to store code\n");
        return false;
    }

    auto pathName = path + "/name";
    if(!app.storage->store(pathName.c_str(), name)) {
        printf("[EffectsManager] failed to store name\n");
        return false;
    }

    app.storage->store("cfg/ef_last_reg", id);
    lastRegisteredId = id;
    registerNewEffect(id, name);
    printf("[EffectsManager] added new effect with id: %d name: %s\n", id, name.c_str());

    return true;
}

std::string EffectsManager::fetchEffectCode(uint32_t id)
{
    auto path = std::string("eff/e_") + std::to_string(id) + "/code";

    return app.storage->read_string(path.c_str());
}

void EffectsManager::registerNewEffect(int id, const std::string &name)
{
    EffectDescription effect;
    effect.name = name;
    effect.id = id;
    effect.type = EffectType::USER_DEFINED;

    effects.emplace_back(std::move(effect));
}
