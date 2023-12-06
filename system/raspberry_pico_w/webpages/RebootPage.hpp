#pragma once

#include "WebServer.hpp"
#include "Application.hpp"

class RebootPage {
public:
    RebootPage(Application& app);
    Response handle(const Request& request);
    RequestHandler getHandler();
private:
    Application app;

    void handleRebootAgreement(const Request& request, Response& response);
};