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

    readAllStoredEffects();

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

void EffectsManager::reloadCurrentEffect(){
    effectChanged = true;
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

    for(const auto& eff: effects) {
        if(eff.id == selectedEffect) {
            for(const auto& param: eff.parameters) {
                app.animationEngine->setColorParameterValue(param.name, param.value);
            }
        }
    }
}

bool EffectsManager::addNewEffect(const std::string &name_, const std::string &code)
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

    //Change animation is a hack to get code of new added python module to be running in python execution. There is no easy way to get duplicate python engine
    app.animationEngine->changeAnimation(code);
    auto name = app.animationEngine->getModuleName();

    auto pathName = path + "/name";
    if(!app.storage->store(pathName.c_str(), name)) {
        printf("[EffectsManager] failed to store name\n");
        return false;
    }

    auto parameters = app.animationEngine->getParameters();
    auto paramName = path + "/params";
    if(!storeParameters(paramName.c_str(), parameters)){
        printf("[EffectsManager] failed to store effect parameters\n");
        return false;
    }

    app.effectsManager->reloadCurrentEffect();

    app.storage->store("cfg/ef_last_reg", id);
    lastRegisteredId = id;
    registerNewEffect(id, name, parameters);
    printf("[EffectsManager] added new effect with id: %d name: %s\n", id, name.c_str());

    return true;
}

bool EffectsManager::removeEffect(uint32_t id)
{
    for(int i = 0; i < effects.size(); ++i) {
        const auto& effect = effects[i];
        if(effect.id == id && effect.type == EffectType::USER_DEFINED) {
            std::string path = std::string("eff/e_") + std::to_string(effect.id);
            auto codePath = path + "/code";
            auto namePath = path + "/name";
            auto paramsPath = path + "/params";
            app.storage->remove(codePath.c_str());
            app.storage->remove(namePath.c_str());
            app.storage->remove(paramsPath.c_str());
            app.storage->remove(path.c_str());
            effects.erase(effects.begin() + i);

            if(selectedEffect == id)
                selectedEffect = 0;
            return true;
        }
    }
    return false;
}

bool EffectsManager::setParameterForEffect(uint32_t id, const std::string &name, const std::string &value)
{
    for(auto& eff: effects) {
        if(eff.id == id) {
            for(auto& desc : eff.parameters) {
                if(desc.name == name) {
                    desc.value = value;

                    if(id == selectedEffect){
                        app.animationEngine->setColorParameterValue(name, value);
                    }

                    auto path = std::string("eff/e_") + std::to_string(id) + "/params";
                    storeParameters(path.c_str(), eff.parameters);

                    return true;
                }
            }
        }
    }

    return false;
}

std::string EffectsManager::fetchEffectCode(uint32_t id)
{
    auto path = std::string("eff/e_") + std::to_string(id) + "/code";

    return app.storage->read_string(path.c_str());
}

void EffectsManager::registerNewEffect(int id, const std::string &name, const std::vector<ParameterDescription>& parameters)
{
    EffectDescription effect;
    effect.name = name;
    effect.id = id;
    effect.type = EffectType::USER_DEFINED;
    effect.parameters = parameters;

    printf("[EffectsManager][registerNewEffect]id: %d, name: %s, parameters(num): %d type: USER_DEFINED\n", id, name.c_str(), parameters.size());

    effects.emplace_back(std::move(effect));
}

void EffectsManager::readAllStoredEffects()
{
    printf("[EffectsManager]readAllStoredEffects\n");
    app.storage->iterateOverDirectory("eff", reinterpret_cast<Storage::DirectoryIteratorFunc>(&EffectsManager::storedDirEntry), this);
    printf("[EffectsManager]readAllStoredEffects done.\n");
}


static void printDebug(const ParameterDescription pd) {
    printf("Type: Color, name: %s, defaultValue %s, value %s \n", pd.name.c_str(), pd.defaultValue.c_str(), pd.value.c_str());
}

std::vector<ParameterDescription> EffectsManager::readEffectParameters(const char* path) {
    std::vector<ParameterDescription> paramsOut;
    auto params = app.storage->read_string(path);
    //printf("readEffectParameters --- path: %s\n", path);
    //printf("params: %s\n", params.c_str());
    if(params.empty())
        return paramsOut;

    auto end = params.find("\n");
    if(end == std::string::npos)
        return paramsOut;

    auto numberOfParameters = std::stoi(params.substr(0, end));
    //printf("numberOfParameters: %s\n", params.substr(0, end).c_str());
    for(int i = 0; i < numberOfParameters; ++i) {
        auto begin = end+1;
        end = params.find("\n", begin);
        if(end == std::string::npos)
            return paramsOut;

        // auto type = params.substr(begin, end-begin);
        // printf("type: %s\n", type.c_str());
        // if(type[0] != 'e')
        //     return paramsOut;

        begin = end+1;
        end = params.find("\n", begin);
        if(end == std::string::npos)
            return paramsOut;

        auto name = params.substr(begin, end-begin);
        //printf("name: %s\n", name.c_str());
        begin = end+1;
        end = params.find("\n", begin);
        if(end == std::string::npos)
            return paramsOut;

        auto defaultValue = params.substr(begin, end-begin);
        //printf("defaultValue: %s\n", defaultValue.c_str());
        begin = end+1;
        end = params.find("\n", begin);
        if(end == std::string::npos)
            return paramsOut;

        auto value = params.substr(begin, end-begin);
        //printf("value: %s\n", value.c_str());

        ParameterDescription p;
        p.type = ParameterType::Color;
        p.name = name;
        p.defaultValue = defaultValue;
        p.value = value;
        paramsOut.push_back(p);
        printDebug(p);
    }

    return paramsOut;
}

void EffectsManager::storedDirEntry(const Storage::DirEntryInfo &entry)
{
    printf("[EffectsManager][storedDirEntry]entry name %s\n", entry.name);
    std::string entryName = entry.name;
    if(entryName.find("e_") == 0) {
        std::string idString = entryName.substr(2);
        if(idString.size() == 0)
            return;
        uint32_t id = std::stoi(idString);

        auto path = std::string("eff/") + entryName + "/name";
        auto effectName = app.storage->read_string(path.c_str());
        auto pathParameters = std::string("eff/") + entryName + "/params";
        auto parameters = readEffectParameters(pathParameters.c_str());

        registerNewEffect(id, effectName, parameters);
    }
}

bool EffectsManager::storeParameters(const char* path, const std::vector<ParameterDescription> &parameters)
{
    std::string txt = std::to_string(parameters.size()) + "\n";
    for(const auto& param : parameters) {
        txt += std::to_string((int)param.type) + "\n";
        txt += param.name + "\n";
        txt += param.defaultValue + "\n";
        txt += param.value + "\n";
    }

    return app.storage->store(path, txt);
}
