#include "EffectsPage.hpp"

#include "EffectsManager.hpp"

EffectsPage::EffectsPage(Application &app)
    :app(app)
{}

Response EffectsPage::handle(const Request &request)
{
    Response response;

    response.headers.push_back(Headers::ContentType::text_html);

    response.write("Effects list:");
    response.write("<ul>");
    for(const auto& effect: app.effectsManager->getListOfEffects()) {
        response.write(std::string("<li><a href=\"/effects/select/") + std::to_string(effect.id) + "\">" + effect.name + "</a></li>");
    }
    response.write("</ul>");

    return response;
}

RequestHandler EffectsPage::getHandler()
{
    RequestHandler req;
    req.uri = "/effects";
    req.handlerObject = this;
    req.handler = reinterpret_cast<RequestHandlerFunc>(&EffectsPage::handle);

    return req;
}