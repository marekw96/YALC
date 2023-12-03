#include "IndexPage.hpp"
#include "static/header.html.hpp"
#include "static/footer.html.hpp"

IndexPage::IndexPage(Application &app)
    :app(app)
{
}

Response IndexPage::handle(const Request &request)
{
    Response response;
    response.headers.push_back(Headers::ContentType::text_html);
    response.write(header_html);

    response.write("Welcome to Yet Another Led Controller<br />");
    response.write("Github project page: <a href=\"https://github.com/marekw96/YALC/tree/YALC2\">YALC2</a><br />");

    response.write(footer_html);
    return response;
}

RequestHandler IndexPage::getHandler()
{
    RequestHandler req;
    req.uri = "/";
    req.handlerObject = this;
    req.handler = reinterpret_cast<RequestHandlerFunc>(&IndexPage::handle);

    return req;
}
