#pragma once

#include <WebServer.h>

#include "effect_information.hpp"

class web_interface {
public:
    web_interface();
    bool initialise();
    void set_effects_info(effect_information* infos);
    void periodic();

private:
    void on_index();

    WebServer server;
    effect_information* infos;
};