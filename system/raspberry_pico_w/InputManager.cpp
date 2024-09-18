#include "InputManager.hpp"

InputManager::InputManager(Application &app)
    : app(app)
{}

bool InputManager::init()
{
    return true;
}

void InputManager::registerInput(Input input)
{
    this->inputs.push_back(input);
}

bool InputManager::enable(const std::string &name)
{
    return this->enable(this->get_id(name));
}

bool InputManager::enable(uint32_t id)
{
    if(id <= inputs.size()){
        auto& input = inputs[id];

        if(input.init(input.obj)) {
            input.isEnabled = true;
            printf("[InputManager] enabling %s %d\n", input.description.name.c_str(),  id);
            return true;
        }
    }

    return false;
}

bool InputManager::disable(const std::string &name)
{
    return this->disable(this->get_id(name));
}

bool InputManager::disable(uint32_t id)
{
    if(id <= inputs.size()){
        auto& input = inputs[id];

        if(input.deinit(input.obj)) {
            input.isEnabled = false;
            printf("[InputManager] disabling %s %d\n", input.description.name.c_str(),  id);
            return true;
        }
    }

    return false;
}

bool InputManager::disableAll()
{
    bool allDone = true;
    for(auto i = 0u; i < inputs.size(); ++i){
        if(!this->disable(i))
            allDone = false;
    }

    return allDone;
}

const std::vector<Input> &InputManager::getInputs()
{
    return this->inputs;
}

std::string InputManager::getValue(const std::string &name)
{
    auto id = get_id(name);
    if(id < 0){
        return "";
    }

    return inputs[id].getValue(inputs[id].obj);
}

const Input *InputManager::getInput(const std::string &name)
{
    auto id = get_id(name);
    if(id < 0)
        return nullptr;
    return &inputs[id];
}

uint32_t InputManager::get_id(const std::string &name)
{
    for(uint32_t i = 0; i < this->inputs.size(); ++i){
        if(this->inputs[i].description.name == name)
            return i;
    }

    return -1;
}
