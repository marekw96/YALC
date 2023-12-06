#include "LedsConfigurationPage.hpp"

#include "static/header.html.hpp"
#include "static/footer.html.hpp"

#include "LedsConfiguration.hpp"

LedsConfigurationPage::LedsConfigurationPage(Application &app)
    :app(app)
{
}

Response LedsConfigurationPage::handle(const Request &request)
{
    Response response;
    response.headers.push_back(Headers::ContentType::text_html);
    response.write(header_html);

    if(request.uri.find("/leds/change") == 0){
        handleChaningPixelsNumber(request, response);
    }

    response.write("Number of pins avaliable: ").write(std::to_string(app.ledsConfiguration->getNumberOfPins())).write("<br />");
    response.write("LED type: ws2812b<br />");
    response.write("<form method=\"POST\" action=\"/leds/change\">"
                   "PIN 0: <input type=\"number\" name=\"pin0\" value=\"").write(std::to_string(app.ledsConfiguration->getPixelsFor(0))).write("\" /><br />"
                   "PIN 1: <input type=\"number\" name=\"pin1\" value=\"").write(std::to_string(app.ledsConfiguration->getPixelsFor(1))).write("\" /><br />"
                   "<button>Change</button>");

    response.write(footer_html);
    return response;
}

RequestHandler LedsConfigurationPage::getHandler()
{
    RequestHandler req;
    req.uri = "/leds/*";
    req.handlerObject = this;
    req.handler = reinterpret_cast<RequestHandlerFunc>(&LedsConfigurationPage::handle);

    return req;
}

void LedsConfigurationPage::handleChaningPixelsNumber(const Request &request, Response &response)
{
    auto pin0 = request.getParameter("pin0");
    auto pin1 = request.getParameter("pin1");

    if(!pin0.isOk() || !pin1.isOk()) {
        response.write("Please provide values<br />");
        return;
    }

    auto pin0value = std::stoi(pin0->value);
    auto pin1value = std::stoi(pin1->value);

    app.ledsConfiguration->setPixelsFor(0, pin0value);
    app.ledsConfiguration->setPixelsFor(1, pin1value);

    response.write("Update leds lenghts<br />");
    response.write("<strong>Changes will be applied after reboot</strong><br />");
}
