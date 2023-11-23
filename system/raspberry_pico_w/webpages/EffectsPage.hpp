#pragma once

#include "WebServer.hpp"
#include "Application.hpp"

class EffectsPage {
public:
    EffectsPage(Application& app);
    Response handle(const Request& request);
    RequestHandler getHandler();
private:
    Application app;

    void handleSelectingNewEffect(const Request& request, Response& response);
};