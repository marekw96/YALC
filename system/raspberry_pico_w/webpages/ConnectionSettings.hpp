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

    void handleChangeSTA(const Request& request, Response& response);
    void handleChangeAP(const Request& request, Response& response);
    void handleConnectSTA(const Request& request, Response& response);
    void handleStartAP(const Request& request, Response& response);
};