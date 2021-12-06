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
    if(this->server.method() == HTTP_GET)
    {
        Serial.println("GET");
        for (uint8_t i=0; i<server.args(); i++){
            Serial.println(server.argName(i));
            Serial.println(server.arg(i));
            if(server.argName(i) == "id")
            {
                if(this->effects->change_effect_to(server.arg(i).c_str()))
                {
                    Serial.println("Changed to!");
                }
                else
                {
                    Serial.println("Failed to change.");
                }
            }
        }
    }

    Serial.println("web_interface::on_index");
    String txt = "";

    for(int i = 0; i < this->effects->get_number_of_effects(); ++i)
    {
        txt += "<a href=\"?id=";
        txt += this->effects->get_effects()[i].id;
        txt += "\">";
        txt += this->effects->get_effects()[i].name;
        txt += "</a><br />";
    }

    server.send(200, "text/html", txt);
}

void web_interface::periodic()
{
    server.handleClient();
}