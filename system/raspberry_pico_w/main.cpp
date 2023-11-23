#include <stdio.h>
#include "pico/stdlib.h"

#include "Time.hpp"
#include "ws2812/ws2812.hpp"
#include "ws2812/LedString_ws2812.hpp"
#include "../pythonAnimations/AnimationEngine.hpp"

#include "InternetManager.hpp"
#include "WebServer.hpp"
#include "lwip/init.h"
#include "pico/cyw43_arch.h"
#include "pico/multicore.h"
#include "Application.hpp"
#include "EffectsManager.hpp"

#include "webpages/ConnectionSettings.hpp"
#include "webpages/EffectsPage.hpp"

Application application;

void core_with_python_animations(){
    printf("[core_with_python_animations] starting\n");
    Time time;
    LedString_ws2812 ledString(1, 100);

    auto* vm = new AnimationEngine(ledString);

    vm->init();
    vm->createAnimation(application.effectsManager->getEffectCode(application.effectsManager->getSelectedEffectId()));

    auto current = time.current();
    while(true) {
        ledString.update();
        auto diff = time.current() - current;
        vm->periodic(diff);

        current = time.current();
    }

    delete vm;
}

void core_with_non_rt_stuff() {
    printf("[core_with_non_rt_stuff] starting\n");
    Time time;
    InternetManager internet;
    WebServer webServer;


    application.internetManager = &internet;


    internet.init();
    if(internet.connect_to("SSID", "passwd")){
        printf("Unable to connect\n");

    }
    else {
        printf("Connected to wifi\n");
    }

    internet.start();
    webServer.init(80);

    ConnectionSettingsPage connectionSettingsPage(application);
    EffectsPage effectsPage(application);
    webServer.registerHandler(connectionSettingsPage.getHandler());
    webServer.registerHandler(effectsPage.getHandler());

    auto current = time.current();
    while(true) {
        auto diff = time.current() - current;
        current = time.current();
        internet.periodic();

        if(application.effectsManager->hasEffectChanged()) {
            multicore_reset_core1();
            multicore_launch_core1(core_with_python_animations);
            application.effectsManager->restartedEffects();
        }
    }

    internet.deinit();
}

int main() {
    stdio_init_all();

    EffectsManager effectsManager(application);
    application.effectsManager = &effectsManager;

    multicore_launch_core1(core_with_python_animations);
    core_with_non_rt_stuff();

    return 0;
}