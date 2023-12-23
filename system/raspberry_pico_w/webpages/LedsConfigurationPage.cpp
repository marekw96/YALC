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

    auto printPin = [&](uint32_t id) {
        response.write("PIN ").write(std::to_string(id))
                .write(" <input type=\"number\" name=\"pin").write(std::to_string(id)).write("\" value=\"").write(std::to_string(app.ledsConfiguration->getPixelsFor(id))).write("\" />")
                .write("<select name=\"colorConfig").write(std::to_string(id)).write("\">")
                .write("<option value=\"RGB\"").write(app.ledsConfiguration->getColorConfig(id) == LedColorConfig::RGB?"selected/":"").write(">RGB</option>")
                .write("<option value=\"GRB\"").write(app.ledsConfiguration->getColorConfig(id) == LedColorConfig::GRB?"selected/":"").write(">GRB</option>")
                .write("<option value=\"BGR\"").write(app.ledsConfiguration->getColorConfig(id) == LedColorConfig::BGR?"selected/":"").write(">BGR</option>")
                .write("</select>")
                .write("<br />");
    };

    response.headers.push_back(Headers::ContentType::text_html);
    response.write(header_html);

    if(request.uri.find("/leds/change") == 0){
        handleChaningPixelsNumber(request, response);
    }

    response.write("Number of pins avaliable: ").write(std::to_string(app.ledsConfiguration->getNumberOfPins())).write("<br />");
    response.write("LED type: ws2812b<br />");
    response.write("<form method=\"POST\" action=\"/leds/change\">");
    printPin(0);
    printPin(1);
    response.write("<button>Change</button>");

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

    auto cc0 = request.getParameter("colorConfig0");
    auto cc1 = request.getParameter("colorConfig1");

    if(!pin0.isOk() || !pin1.isOk() || !cc0.isOk() || !cc1.isOk()) {
        response.write("Please provide values<br />");
        return;
    }

    auto pin0value = std::stoi(pin0->value);
    auto pin1value = std::stoi(pin1->value);

    app.ledsConfiguration->setPixelsFor(0, pin0value);
    app.ledsConfiguration->setPixelsFor(1, pin1value);

    if(cc0->value == "RGB"){
        app.ledsConfiguration->setColorConfigFor(0, LedColorConfig::RGB);
    }
    else if(cc0->value == "BGR"){
        app.ledsConfiguration->setColorConfigFor(0, LedColorConfig::BGR);
    }
    else {
        app.ledsConfiguration->setColorConfigFor(0, LedColorConfig::GRB);
    }

    if(cc1->value == "RGB"){
        app.ledsConfiguration->setColorConfigFor(1, LedColorConfig::RGB);
    }
    else if(cc1->value == "BGR"){
        app.ledsConfiguration->setColorConfigFor(1, LedColorConfig::BGR);
    }
    else {
        app.ledsConfiguration->setColorConfigFor(1, LedColorConfig::GRB);
    }

    response.write("Update leds lenghts<br />");
    response.write("<strong>Changes will be applied after reboot</strong><br />");
}
