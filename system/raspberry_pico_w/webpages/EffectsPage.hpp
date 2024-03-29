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
    bool handleAddingNewEffect(const Request& request, Response& response);
    void handleRemovingEffect(const Request& request, Response& response);
    void handlechangeParameters(const Request& request, Response& response);
    bool handleRemovingEffectQuestion(const Request& request, Response& response);
    void handleChangeRotation(const Request& request, Response& response);
};