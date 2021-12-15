#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#include "esp_timer.h"

#include "web_interface.hpp"
#include "config.hpp"

#include "effect_manager.hpp"
#include "effect_information.hpp"
#include "config_manager.hpp"

web_interface web;

Adafruit_NeoPixel pixels(25, 16, NEO_GRB + NEO_KHZ800);
effect_manager effects;
config_manager cfg_manager;

int64_t last_time_read;

void setup() {
    Serial.begin(115200);

    esp_timer_init();
    Serial.println("Timer init done.");

    if(cfg_manager.begin())
        Serial.println("CFG manager done.");
    else
        Serial.println("CFG manager begin failed.");

    Serial.println("Connecting to ");
    Serial.println(wifi_ssid);

    WiFi.begin(wifi_ssid, wifi_password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("WiFi connected..!");
    Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

    web.initialise();
    web.set_effect_manager(effects);
    Serial.println("HTTP server started");

    pixels.begin();

    effects.set_pixels(pixels);
    effects.set_config_manager(cfg_manager);
    effects.get_current_effect().init(pixels);
    effects.restore_effect_parameters();

    last_time_read = esp_timer_get_time();
}

void loop() {
    web.periodic();

    int64_t time_read = esp_timer_get_time();
    effects.get_current_effect().periodic(time_read - last_time_read);

    last_time_read = time_read;
}
