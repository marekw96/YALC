#include "RebootPage.hpp"

#include "static/header.html.hpp"
#include "static/footer.html.hpp"

#include "hardware/watchdog.h"

RebootPage::RebootPage(Application &app)
    :app(app)
{
}

Response RebootPage::handle(const Request &request)
{
    Response response;
    response.headers.push_back(Headers::ContentType::text_html);
    response.write(header_html);

    if(request.uri.find("/reboot/yes") == 0){
        handleRebootAgreement(request, response);
    }else {
        response.write("Click <a href=\"reboot/yes\">HERE</a> to reboot device<br />");
    }

    response.write(footer_html);
    return response;
}

RequestHandler RebootPage::getHandler()
{
    RequestHandler req;
    req.uri = "/reboot/*";
    req.handlerObject = this;
    req.handler = reinterpret_cast<RequestHandlerFunc>(&RebootPage::handle);

    return req;
}

void RebootPage::handleRebootAgreement(const Request &request, Response &response)
{
    response.write("<strong>Device will be rebooted</strong><br />");
    app.shouldBeRebooted = true;
    watchdog_reboot(0, SRAM_END, 350);
}
