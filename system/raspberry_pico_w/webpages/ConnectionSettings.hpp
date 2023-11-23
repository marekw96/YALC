#pragma once

#include "WebServer.hpp"
#include "Application.hpp"

class ConnectionSettingsPage {
public:
    ConnectionSettingsPage(Application& app);
    Response handle(const Request& request);
    RequestHandler getHandler();

private:
    Application app;
};