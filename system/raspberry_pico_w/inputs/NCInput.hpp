#pragma once

#include "InputManager.hpp"

class NCInput {
public:
    NCInput(const std::string& name, InputType type)
    :   name(name), type(type) {}

    Input getHandler() {
        Input input;
        input.description.name = this->name;
        input.description.type = type;

        input.obj = this;
        input.init = reinterpret_cast<bool(*)(void*)>(&NCInput::init);
        input.deinit = reinterpret_cast<bool(*)(void*)>(&NCInput::deinit);
        input.getValue = reinterpret_cast<std::string(*)(void*)>(&NCInput::getValue);

        return input;
    }

    bool init(){
        return true;
    }

    bool deinit(){
        return true;
    }

    std::string getValue(){
        return "";
    }

private:
    std::string name;
    InputType type;
};