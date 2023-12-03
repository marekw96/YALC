#pragma once

#include "WebServer.hpp"
#include "Application.hpp"

class IndexPage {
public:
    IndexPage(Application& app);
    Response handle(const Request& request);
    RequestHandler getHandler();
private:
    Application app;
};