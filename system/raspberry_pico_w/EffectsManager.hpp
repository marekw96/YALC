#pragma once

#include "Application.hpp"

#include <string>
#include <vector>
#include "Storage.hpp"

#include "../pythonAnimations/Parameter.hpp"

enum class EffectType {
    BUILTIN, USER_DEFINED
};

struct EffectDescription {
    uint32_t id;
    std::string name;
    EffectType type;
    std::vector<ParameterDescription> parameters;
};

class EffectsManager {
public:
    EffectsManager(Application& app);
    bool init();
    void periodic(Duration diff);

    std::vector<EffectDescription> getListOfEffects();
    std::string getEffectCode(uint32_t id);
    bool selectEffect(uint32_t newId);
    uint32_t getSelectedEffectId();

    bool hasEffectChanged();
    void reloadCurrentEffect();
    void restartedEffects();
    bool addNewEffect(const std::string& code);
    bool removeEffect(uint32_t id);

    bool setParameterForEffect(uint32_t id, const std::string& name, const std::string& value);

    uint32_t getNextAnimationTimeout() const;
    void setNextAnimationTimeout(uint32_t miliseconds);
private:
    Application& app;
    uint32_t selectedEffect = 0;
    uint32_t lastRegisteredId = 2;
    bool effectChanged = false;
    std::vector<EffectDescription> effects = {{0,"MyAnimation", EffectType::BUILTIN}, {1,"FallingStar", EffectType::BUILTIN}};
    uint32_t nextAnimationTimeoutMs = 0;
    uint32_t nextAnimationTimeElapsedUs = 0;

    std::string fetchEffectCode(uint32_t id);
    void registerNewEffect(int id, const std::string& name, const std::vector<ParameterDescription>& parameters);
    void readAllStoredEffects();
    void storedDirEntry(const Storage::DirEntryInfo& entry);
    bool storeParameters(const char* path, const std::vector<ParameterDescription> &parameters);
    std::vector<ParameterDescription> readEffectParameters(const char* path);
};