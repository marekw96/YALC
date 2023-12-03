#include "ConnectionSettings.hpp"

#include "InternetManager.hpp"

#include "static/header.html.hpp"
#include "static/footer.html.hpp"

ConnectionSettingsPage::ConnectionSettingsPage(Application &app)
    :app(app)
{}

Response ConnectionSettingsPage::handle(const Request &request)
{
    Response response;

    response.headers.push_back(Headers::ContentType::text_html);
    response.write(header_html);

    if(request.uri.find("/connectionSettings/changeSTA") == 0){
        handleChangeSTA(request, response);
    }
    else if(request.uri.find("/connectionSettings/changeAP") == 0){
        handleChangeAP(request, response);
    }
    else if(request.uri.find("/connectionSettings/connectToSTA") == 0){
        handleConnectSTA(request, response);
    }
    else if(request.uri.find("/connectionSettings/startAP") == 0){
        handleStartAP(request, response);
    }

    response.write("Status: ");
    response.write(to_char(app.internetManager->getStatus()));
    response.write("<br />");

    response.write("Current preffered network: ").write(to_char(app.internetManager->getPrefferedNetworkType())).write("<br />");

    response.write("<a href=\"/connectionSettings/connectToSTA\">Connect to network</a> <a href=\"/connectionSettings/startAP\">Start AP</a><br />");

    response.write("<form method=\"POST\" action=\"/connectionSettings/changeSTA\">")
            .write("Network SSID:<input name=\"ssid\" value=\"").write(app.internetManager->getSTAName()).write("\"><br />")
            .write("Network password:<input name=\"passwd\" type=\"password\"><br />")
            .write("Security:<select name=\"auth\">")
            .write("<option value=\"none\"").write(app.internetManager->getAuth() == NetworkAuth::NONE?" selected":"").write(">Open</option>")
            .write("<option value=\"wpa\"").write(app.internetManager->getAuth() == NetworkAuth::WPA?" selected":"").write(">WPA</option>")
            .write("<option value=\"wpa2\"").write(app.internetManager->getAuth() == NetworkAuth::WPA2_AES?" selected":"").write(">WPA2 AES</option>")
            .write("<option value=\"wpaM\"").write(app.internetManager->getAuth() == NetworkAuth::WPA2_MIXED?" selected":"").write(">WPA2 mixed</option>")
            .write("</select>")
            .write("<button>Change</button></form><br />");

    response.write("<form method=\"POST\" action=\"/connectionSettings/changeAP\">")
            .write("Network SSID:<input name=\"ssid\" value=\"").write(app.internetManager->getAPName()).write("\"><br />")
            .write("Network password:<input name=\"passwd\" value=\"").write(app.internetManager->getAPPassword()).write("\"><br />")
            .write("Security: ").write(to_char(app.internetManager->getAPPAuth())).write("<br />")
            .write("<button>Change</button></form><br />");

    response.write(footer_html);
    return response;
}

RequestHandler ConnectionSettingsPage::getHandler()
{
    RequestHandler req;
    req.uri = "/connectionSettings/*";
    req.handlerObject = this;
    req.handler = reinterpret_cast<RequestHandlerFunc>(&ConnectionSettingsPage::handle);

    return req;
}

void ConnectionSettingsPage::handleChangeSTA(const Request &request, Response &response)
{
    auto ssidParam = request.getParameter("ssid");
    auto passwdParam = request.getParameter("passwd");
    auto authParam = request.getParameter("auth");

    std::string ssid;
    std::string passwd;
    NetworkAuth auth = NetworkAuth::NONE;

    if(!ssidParam) {
        response.write("You need to provide ssid<br />");
        return;
    }

    if(passwdParam) {
        passwd = passwdParam->value;
    }

    if(authParam) {
        auto val = authParam->value;
        if(val == "wpa")
            auth = NetworkAuth::WPA;
        else if(val == "wpa2")
            auth = NetworkAuth::WPA2_AES;
        else if(val == "wpaM")
            auth = NetworkAuth::WPA2_MIXED;
        else
            auth = NetworkAuth::NONE;
    }

    app.internetManager->storeSTANetowrkData(ssidParam->value, passwd, auth);
    response.write("Changed STA data<br />");
}

void ConnectionSettingsPage::handleChangeAP(const Request &request, Response &response)
{
    auto ssidParam = request.getParameter("ssid");
    auto passwdParam = request.getParameter("passwd");
    auto auth = NetworkAuth::NONE;

    std::string ssid;
    std::string passwd;

    if(!ssidParam) {
        response.write("You need to provide ssid<br />");
        return;
    }

    if(passwdParam) {
        passwd = passwdParam->value;
        auth = NetworkAuth::WPA2_MIXED;
    }

    app.internetManager->storeAPData(ssidParam->value, passwd, auth);
    response.write("Changed AP data<br />");
}

void ConnectionSettingsPage::handleConnectSTA(const Request& request, Response& response)
{
    app.internetManager->changeNetworkType(NetworkType::STANDALONE);

    printf("[ConnectionSettingsPage]Trying to connect to STA\n");
    response.write("Trying to connect to STA<br />");
}

void ConnectionSettingsPage::handleStartAP(const Request& request, Response& response)
{
    app.internetManager->changeNetworkType(NetworkType::ACCESS_POINT);

    printf("[ConnectionSettingsPage]Trying to start an AP\n");
    response.write("Trying to start an AP<br />");
}
