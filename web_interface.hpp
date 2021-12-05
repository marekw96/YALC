#pragma once

#include <WebServer.h>

#include "effect_manager.hpp"

class web_interface {
public:
    web_interface();
    bool initialise();
    void set_effect_manager(effect_manager& effects);
    void periodic();

private:
    void on_index();

    WebServer server;
    effect_manager* effects;
};