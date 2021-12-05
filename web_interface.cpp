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

void web_interface::set_effect_manager(effect_manager& effects)
{
    this->effects = &effects;
}

void web_interface::on_index()
{
    Serial.println("web_interface::on_index");
    String txt = "";
Serial.println(this->effects->get_number_of_effects());
    for(int i = 0; i < this->effects->get_number_of_effects(); ++i)
    {
        txt += "<br />";
        txt += this->effects->get_effects()[i].name;
    }

    server.send(200, "text/html", txt);
}

void web_interface::periodic()
{
    server.handleClient();
}