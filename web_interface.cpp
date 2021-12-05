#include "web_interface.hpp"

#include <functional>

web_interface::web_interface()
    : server(80)
{}

bool web_interface::initialise()
{
    server.on("/", std::bind(&web_interface::on_index, this));

    this->server.begin();
    return true;
}

void web_interface::set_effects_info(effect_information* infos)
{
    this->infos = infos;
}

void web_interface::on_index()
{
    Serial.println("web_interface::on_index");
    String txt = this->infos[0].name;
    txt += "<br />";
    txt += this->infos[1].name;

    server.send(200, "text/html", txt);
}

void web_interface::periodic()
{
    server.handleClient();
}