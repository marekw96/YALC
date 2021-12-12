#include "web_interface.hpp"

#include <functional>

#include "effect_parameter.hpp"

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
    view<effect_parameter> effect_parameters;
    if(this->server.method() == HTTP_GET)
    {
        bool effect_exists = false;
        String effect_name;
        Serial.println("GET");
        for (uint8_t i=0; i<server.args(); i++) {
            Serial.println(server.argName(i));
            Serial.println(server.arg(i));
            if(server.argName(i) == "id")
            {
                if(this->effects->effect_with_id_exists(server.arg(i).c_str()))
                {
                    effect_exists = true;
                    effect_name = server.arg(i);

                    effect_parameters = this->effects->get_effect_parameters(server.arg(i).c_str());
                }
            }
        }

        if(effect_exists) {
            this->effects->change_effect_to(effect_name.c_str());

            for (uint8_t i=0; i<server.args(); i++) {
                this->effects->set_effect_parameter(server.argName(i), server.arg(i));
            }

            this->effects->change_effect_to(effect_name.c_str()); //Apply new settings if used in init function...
        }
        else {
            Serial.println("Effect doesn't exists");
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

    if(effect_parameters)
    {
        txt += "<form method=\"get\">";
        txt += "<input name=\"id\" type=\"hidden\" value=\"";
        txt += this->effects->get_current_effect().id;
        txt += "\">";
        for(int i =0; i < effect_parameters.size(); ++i)
        {
            auto& effect = effect_parameters[i];
            switch(effect.type) {
                case EFFECT_TYPE::COLOR:
                    txt += effect.name;
                    txt += " <input type=\"color\" name=\"";
                    txt += effect.id;
                    txt += "\" value=\"";
                    txt += effect.default_value;
                    txt += "\"><br />";
                break;
                default:
                    txt += effect.name;
                    txt += "<br />";
                break;
            }
        }
        txt += "<input type=\"submit\" value=\"Apply\"></form>";
    }
    else{
        txt += "No parameter for this effect";
    }

    server.send(200, "text/html", txt);
}

void web_interface::periodic()
{
    server.handleClient();
}