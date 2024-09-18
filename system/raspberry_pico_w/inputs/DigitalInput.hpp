#pragma once

#include "InputManager.hpp"

class DigitalInput {
public:
    DigitalInput(const std::string& name, uint32_t pin)
    :   name(name), pin(pin) {}

    Input getHandler() {
        Input input;
        input.description.name = this->name;
        input.description.type = InputType::DIGITAL;

        input.obj = this;
        input.init = reinterpret_cast<bool(*)(void*)>(&DigitalInput::init);
        input.deinit = reinterpret_cast<bool(*)(void*)>(&DigitalInput::deinit);
        input.getValue = reinterpret_cast<std::string(*)(void*)>(&DigitalInput::getValue);

        return input;
    }

    bool init(){
        gpio_set_dir(this->pin, GPIO_IN);
        return true;
    }

    bool deinit(){
        return true;
    }

    std::string getValue(){
        return std::to_string(gpio_get(this->pin));
    }

private:
    std::string name;
    uint32_t pin;
};