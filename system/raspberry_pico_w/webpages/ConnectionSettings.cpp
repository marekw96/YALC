#include "ConnectionSettings.hpp"

ConnectionSettingsPage::ConnectionSettingsPage(Application &app)
    :app(app)
{}

Response ConnectionSettingsPage::handle(const Request &request)
{
    Response response;

    response.headers.push_back(Headers::ContentType::text_html);

    response.write("Status: ");
    response.write(to_char(app.internetManager->getStatus()));
    response.write("<br />");

    return response;
}

RequestHandler ConnectionSettingsPage::getHandler()
{
    RequestHandler req;
    req.uri = "/connectionSettings";
    req.handlerObject = this;
    req.handler = reinterpret_cast<RequestHandlerFunc>(&ConnectionSettingsPage::handle);

    return req;
}