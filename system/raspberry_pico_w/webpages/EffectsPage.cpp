#include "EffectsPage.hpp"

#include "EffectsManager.hpp"
#include "static/header.html.hpp"
#include "static/footer.html.hpp"


EffectsPage::EffectsPage(Application &app)
    :app(app)
{}

Response EffectsPage::handle(const Request &request)
{
    Response response;
    response.headers.push_back(Headers::ContentType::text_html);
    response.write(header_html);
    bool printContent = true;

    if(request.uri.find("/effects/select/") == 0){
        handleSelectingNewEffect(request, response);
    }
    if(request.uri.find("/effects/add") == 0){
        printContent = handleAddingNewEffect(request, response);
    }
    if(request.uri.find("/effects/remove_check") == 0) {
        printContent = handleRemovingEffectQuestion(request, response);
    }
    if(request.uri.find("/effects/remove_yes") == 0) {
        handleRemovingEffect(request, response);
    }
    if(request.uri.find("/effects/changeParameters") == 0) {
        handlechangeParameters(request, response);
    }
    if(request.uri.find("/effects/rotate") == 0) {
        handleChangeRotation(request, response);
    }

    if(printContent){
        auto writeOption = [&](uint32_t ms, const std::string& name){
            response.write("<option value=\"")
                    .write(std::to_string(ms)).write("\" ").write(app.effectsManager->getNextAnimationTimeout() == ms?"selected":"").write(">").write(name).write("</option>");
        };

        response.write("Change animation to next user defined animation after: ");
        response.write("<form method=\"POST\" action=\"/effects/rotate\">")
                .write("<select name=\"timeout\">");
        writeOption(0, "OFF");
        writeOption(5*1000, "5sec");
        writeOption(10*1000, "10sec");
        writeOption(15*1000, "15sec");
        writeOption(30*1000, "30sec");
        writeOption(60*1000, "1min");
        writeOption(3*60*1000, "3min");
        writeOption(5*60*1000, "5min");
        writeOption(10*60*1000, "10min");
        response.write("</select>")
                .write("<button>Set</button>")
                .write("</form><br /><br />");

        response.write("<a href=\"/effects/add\">Add new animation</a><br /><br />");
        response.write("Effects list:");
        response.write("<ul>");
        for(const auto& effect: app.effectsManager->getListOfEffects()) {
            response.write("<li>");
            response.write(std::string("<a href=\"/effects/select/") + std::to_string(effect.id) + "\">" + effect.name + "</a>");
            if(effect.id != app.effectsManager->getSelectedEffectId())
                response.write(std::string("  <a href=\"/effects/remove_check/") + std::to_string(effect.id) + "\">X</a>");

            if(effect.parameters.size() > 0) {
                response.write("<br /><form method=\"POST\" action=\"/effects/changeParameters/")
                        .write(std::to_string(effect.id)).write("\">");

                for(const auto& param: effect.parameters){
                    response.write(param.name).write(": ")
                        .write("<input type=\"color\" name=\"").write(param.name).write("\" value=\"#")
                        .write(param.value).write("\" /><br />");
                }

                response.write("<button>Store</button></form>");
            }
            response.write("</li>");
        }
        response.write("</ul>");
    }

    response.write(footer_html);

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

bool EffectsPage::handleAddingNewEffect(const Request &request, Response &response)
{
    std::string code = "";
    for(const auto& parameter: request.parameters){
        if(parameter.name == "code")
            code = parameter.value;
    }

    if(code.size() == 0){
        response.write("Add new:<form method=\"POST\" action=\"/effects/add\">");
        response.write("Content: <textarea name=\"code\" rows=\"20\" cols=\"70\"></textarea><br />");
        response.write("<button>Add</button><br />");
        response.write("</form>");

        return false;
    }

    auto result = app.effectsManager->addNewEffect(code);

    if(result)
        response.write("Added new effect!<br />");
    else
        response.write("Failed to add new effect!<br />");

    return true;
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

bool EffectsPage::handleRemovingEffectQuestion(const Request &request, Response &response)
{
    auto part = request.uri.rfind("/");
    if(part == request.uri.length()-1) {
        response.write("Unable to  effect<br />");
        return true;
    }

    auto id_str = request.uri.substr(part+1);
    uint32_t id = std::atoi(id_str.c_str());

    if(id == app.effectsManager->getSelectedEffectId()) {
        response.write("Cannot remove selected effect<br />");
        return true;
    }

    response.write("Do You want to remove effect? Click <a href=\"/effects/remove_yes/").write(std::to_string(id)).write("\">here</a> to remove.<br />");

    return false;
}

void EffectsPage::handlechangeParameters(const Request &request, Response &response)
{
    auto part = request.uri.rfind("/");
    if(part == request.uri.length()-1) {
        response.write("Unable to set effect parameter<br />");
        return;
    }

    auto id_str = request.uri.substr(part+1);
    uint32_t id = std::atoi(id_str.c_str());

    for(const auto& param : request.parameters) {
        printf("[EffectsPage]setting parameter %s for id %d with value %s\n", param.name.c_str(), id, param.value.c_str());
        app.effectsManager->setParameterForEffect(id, param.name, param.value.substr(1));
    }
}

void EffectsPage::handleChangeRotation(const Request &request, Response &response)
{
    auto timeoutParam = request.getParameter("timeout");

    if(!timeoutParam) {
        response.write("Unable to change rotation<br />");
        return;
    }

    uint32_t value = std::atoi(timeoutParam->value.c_str());

    app.effectsManager->setNextAnimationTimeout(value);

    response.write("Changed timeout!<br />");
}
