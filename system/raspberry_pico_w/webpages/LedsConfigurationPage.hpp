#pragma once

#include "WebServer.hpp"
#include "Application.hpp"

class LedsConfigurationPage {
public:
    LedsConfigurationPage(Application& app);
    Response handle(const Request& request);
    RequestHandler getHandler();
private:
    Application app;

    void handleChaningPixelsNumber(const Request& request, Response& response);
};