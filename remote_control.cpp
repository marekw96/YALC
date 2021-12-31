#include "remote_control.hpp"

#include "effect_parameter.hpp"
#include "view.hpp"

#include <WiFiUdp.h>

WiFiUDP udp;

static effect_parameter params[] = {
    {"IP", "ip", "127.0.0.1", EFFECT_TYPE::TEXT},
    {"Port", "port", "7777",  EFFECT_TYPE::TEXT}
};

effect_information remote_control::get_info()
{
    effect_information info;
    info.id = "remote_control";
    info.name = "Remote control";
    info.description = "";
    info.parameters = view<effect_parameter>(params, sizeof(params) / sizeof(params[0]));
    info.init = std::bind(&remote_control::init, this, std::placeholders::_1);
    info.periodic = std::bind(&remote_control::periodic, this, std::placeholders::_1);
    info.deinit = std::bind(&remote_control::deinit, this);
    info.set_parameter = std::bind(&remote_control::set_parameter, this, std::placeholders::_1, std::placeholders::_2);
    info.get_parameter = std::bind(&remote_control::get_parameter, this, std::placeholders::_1);

    return info;
}

void remote_control::init(Adafruit_NeoPixel& pixels) {
    Serial.println("remote_control::init");

    this->pixels = &pixels;
    this->pixels->clear();

    udp.begin(this->port.toInt());
    //Send hello!
    udp.beginPacket(this->ip.c_str(), this->port.toInt());
    const uint8_t data_to_write[] = {'Y','A','L','C', 0,0,1,25};
    udp.write(data_to_write, sizeof(data_to_write)/sizeof(char));
    udp.endPacket();
}

void remote_control::periodic(int64_t time_elapsed) {
    int packet_size = udp.parsePacket();
    if(packet_size) {
        IPAddress remoteIp = udp.remoteIP();
        //Serial.println("GOT!");

        // if(!(remoteIp == IPAddress(reinterpret_cast<const uint8_t*>(this->ip.c_str())))) {
        //     udp.flush();
        //     Serial.print("Not sender ip: ");
        //     Serial.println(remoteIp);
        //     return;
        // }
        if(packet_size != this->pixels->numPixels() * 3) {
            udp.flush();
            Serial.print("Size not correct: ");
            Serial.println(packet_size);
            return;
        }

        char data[3*50];
        udp.read(data, sizeof(data));

        for(int i = 0; i < this->pixels->numPixels(); ++i)
            this->pixels->setPixelColor(i, data[i*3], data[i*3 + 1], data[i*3 + 2]);

        this->pixels->show();
        //Serial.println("GOT!");
    }
}

bool remote_control::set_parameter(const String& name, const String& value) {
    if(name == "ip") {
        this->ip = value;
        return true;
    }
    else if(name == "port") {
        this->port = value;
        return true;
    }

    return false;
}

String remote_control::get_parameter(const String& name) {
    if(name == "ip") {
        return this->ip;
    }
    else if(name == "port") {
        return this->port;
    }

    return {};
}

void remote_control::deinit() {
    udp.stop();
}