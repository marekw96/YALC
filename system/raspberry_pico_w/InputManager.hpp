#pragma once

#include <vector>

#include "Application.hpp"
#include "../pythonAnimations/Parameter.hpp"

struct Input {
    InputDescription description;
    void* obj;
    bool (*init)(void* obj);
    bool (*deinit)(void* obj);
    std::string (*getValue)(void* obj);
    bool isEnabled = false;
};

class InputManager {
public:
    InputManager(Application& app);
    bool init();

    void registerInput(Input input);
    bool enable(const std::string& name);
    bool enable(uint32_t id);
    bool disable(const std::string& name);
    bool disable(uint32_t id);
    bool disableAll();
    const std::vector<Input>& getInputs();
    std::string getValue(const std::string& name);
    const Input* getInput(const std::string& name);

private:
    uint32_t get_id(const std::string& name);

    Application& app;
    std::vector<Input> inputs;
};