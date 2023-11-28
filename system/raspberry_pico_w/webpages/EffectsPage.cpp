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
    if(request.uri.find("/effects/add") == 0){
        handleAddingNewEffect(request, response);
    }
    if(request.uri.find("/effects/remove") == 0) {
        handleRemovingEffect(request, response);
    }

    response.write("Effects list:");
    response.write("<ul>");
    for(const auto& effect: app.effectsManager->getListOfEffects()) {
        response.write("<li>");
        response.write(std::string("<a href=\"/effects/select/") + std::to_string(effect.id) + "\">" + effect.name + "</a>");
        if(effect.id != app.effectsManager->getSelectedEffectId())
            response.write(std::string("  <a href=\"/effects/remove/") + std::to_string(effect.id) + "\">X</a>");
        response.write("</li>");
    }
    response.write("</ul>");

    std::string name = "";
    std::string code = "";

    response.write("<form method=\"POST\" action=\"/effects/add\">");
    response.write(std::string("Name: <input type=\"text\" name=\"name\" value =\"") + name + "\"><br />");
    response.write(std::string("Content: <textarea name=\"code\" rows=\"20\" cols=\"100\">") + code + "</textarea><br />");
    response.write("<button>Add</button><br />");
    response.write("</form>");

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
}

void EffectsPage::handleAddingNewEffect(const Request &request, Response &response)
{
    std::string name = "";
    std::string code = "";
    for(const auto& parameter: request.parameters){
        if(parameter.name == "name")
            name = parameter.value;
        if(parameter.name == "code")
            code = parameter.value;
    }

    if(name.size() == 0 || code.size() == 0){
        response.write("<p>Please provide name and code</p>");
        return;
    }

    auto result = app.effectsManager->addNewEffect(name, code);

    if(result)
        response.write("Added new effect!<br />");
    else
        response.write("Failed to add new effect!<br />");
}

void EffectsPage::handleRemovingEffect(const Request &request, Response &response)
{
    auto part = request.uri.rfind("/");
    if(part == request.uri.length()-1) {
        response.write("Unable to  effect<br />");
        return;
    }

    auto id_str = request.uri.substr(part+1);
    uint32_t id = std::atoi(id_str.c_str());

    if(id == app.effectsManager->getSelectedEffectId()) {
        response.write("Cannot remove selected effect<br />");
        return;
    }

    auto result = app.effectsManager->removeEffect(id);
    if(result) {
        response.write("Effect removed<br />");
    }
    else {
        response.write("Failed during effect removing<br />");
    }
}