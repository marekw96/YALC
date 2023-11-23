#include "EffectsPage.hpp"

#include "EffectsManager.hpp"

EffectsPage::EffectsPage(Application &app)
    :app(app)
{}

Response EffectsPage::handle(const Request &request)
{
    Response response;
    response.headers.push_back(Headers::ContentType::text_html);

    if(request.uri.find("/effects/select/") == 0){
        handleSelectingNewEffect(request, response);
    }

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
    req.uri = "/effects/*";
    req.handlerObject = this;
    req.handler = reinterpret_cast<RequestHandlerFunc>(&EffectsPage::handle);

    return req;
}

void EffectsPage::handleSelectingNewEffect(const Request &request, Response &response)
{
    auto part = request.uri.rfind("/");
    if(part == request.uri.length()-1) {
        response.write("Unable to change effect<br />");
        return;
    }

    auto id_str = request.uri.substr(part+1);
    uint32_t id = std::atoi(id_str.c_str());
    app.effectsManager->selectEffect(id);
    response.write("Changed to ");
    response.write(id_str);
    response.write("<br />");

    app.storage->store("/cfg/eff_selected", id);
}
