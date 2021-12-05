#include <WiFi.h>
#include <Adafruit_NeoPixel.h>

#include "web_interface.hpp"
#include "config.hpp"

#include "blink_all_leds.hpp"
#include "effect_information.hpp"

web_interface web;
blink_all_leds blink_all_leds_obj;

effect_information infos[1];
effect_information* selected = &infos[0];

Adafruit_NeoPixel pixels(25, 13, NEO_GRB + NEO_KHZ800);

void setup() {
    Serial.begin(115200);
    Serial.println("Connecting to ");
    Serial.println(wifi_ssid);

    WiFi.begin(wifi_ssid, wifi_password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    infos[0] = blink_all_leds_obj.get_info();

    Serial.println("WiFi connected..!");
    Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

    web.initialise();
    web.set_effects_info(infos);
    Serial.println("HTTP server started");

    pixels.begin();

    selected->init(pixels);
}

void loop() {
    web.periodic();

    selected->periodic(1);
}
