#include "web_interface.hpp"

#include <functional>

#include "effect_parameter.hpp"
#include "view.hpp"

namespace {
    String page_header() {
        String html = "<!DOCTYPE html>"
                      "<html lang=\"en\">"
                      "<head>"
                      "<title>YALC</title>"
                      "<meta charset=\"utf-8\">"
                      "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                      "</head>"
                      "<body>";
        return html;
    }

    String effects_to_html(view<effect_information> effects, const char* selected_id) {
        String html = "Effects:<br />";
        String selected = selected_id;

        for(int i = 0; i < effects.size(); ++i)
        {
            html += "<a href=\"?id=";
            html += effects[i].id;
            html += "\">";
            html += effects[i].name;
            if(selected == effects[i].id)
                html += " [Selected]";
            html += "</a><br />";
        }

        return html;
    }

    String page_footer() {
        String html = "</body>"
                      "</html>";
        return html;
    }

    String select_paramter_to_html(const effect_parameter& param, const char* selected) {
        String html = "<td>";
        html += param.name;
        html += "</td><td><select name=\"";
        html += param.id;
        html += "\">";

        String values = param.values;
        int last = 0;
        int pos = values.indexOf(';', last);
        while(pos != -1) {
            String value = values.substring(last, pos);

            html += "<option value=\"";
            html += value;
            html += "\"";
            if(value == selected)
                html += " selected";
            html += ">";
            html += value;
            html += "</option>";

            last = pos + 1;
            pos = values.indexOf(';', last);
        }

        html += "</select></td>";

        return html;
    }
}

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
            Serial.printf("Arg name: %s, arg value: %s\n\r", server.argName(i), server.arg(i));
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
            effect_parameters = this->effects->get_current_effect().parameters;
        }
    }
    else {
        effect_parameters = this->effects->get_current_effect().parameters;
    }

    Serial.println("web_interface::on_index");
    String txt = page_header();

    txt += effects_to_html({this->effects->get_effects(), this->effects->get_number_of_effects()}, this->effects->get_current_effect().id);

    if(effect_parameters)
    {
        txt += "<br />Parameters:<br />";
        txt += "<form method=\"get\">";
        txt += "<input name=\"id\" type=\"hidden\" value=\"";
        txt += this->effects->get_current_effect().id;
        txt += "\">";
        txt += "<table with=\"100%\">";
        for(int i =0; i < effect_parameters.size(); ++i)
        {
            txt += "<tr>";
            auto& effect = effect_parameters[i];
            switch(effect.type) {
                case EFFECT_TYPE::COLOR:
                    txt += "<td>";
                    txt += effect.name;
                    txt += "</td><td><input type=\"color\" name=\"";
                    txt += effect.id;
                    txt += "\" value=\"";
                    txt += this->effects->get_effect_parameter(effect.id);
                    txt += "\"></td>";
                break;
                case EFFECT_TYPE::SELECT:
                    txt += select_paramter_to_html(effect, this->effects->get_effect_parameter(effect.id).c_str());
                break;
                default:
                    txt += effect.name;
                    txt += "<br />";
                break;
            }
            txt += "</tr>";
        }
        txt += "</table><input type=\"submit\" value=\"Apply\"></form>";
    }
    else{
        txt += "No parameter for this effect";
    }

    txt += page_footer();

    server.send(200, "text/html", txt);
}

void web_interface::periodic()
{
    server.handleClient();
}