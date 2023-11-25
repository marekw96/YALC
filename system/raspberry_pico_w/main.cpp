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

#include "../pythonAnimations/FallingStar.py.hpp"

Application application;


enum CoreCommunicationStatus {
    BASIC_INIT_DONE = 1,
    RELOAD_ANIMATIONS = 2
};

void core_with_python_animations(){
    printf("[core_with_python_animations] starting\n");
    multicore_lockout_victim_init();
    while(1)
        tight_loop_contents();
    Time time;

    LedString_ws2812 ledString(1, 100);
    AnimationEngine vm(ledString);

    while(true){
        printf("Animation loop started\n");


        ledString.init();
        vm.init();
        //vm.createAnimation(application.effectsManager->getEffectCode(application.effectsManager->getSelectedEffectId()));
        vm.createAnimation(FallingStar_py);

        auto current = time.current();
        while(true) {
            ledString.update();
            auto diff = time.current() - current;
            vm.periodic(diff);

            current = time.current();

            if(application.effectsManager->hasEffectChanged()) {
                application.effectsManager->restartedEffects();
                break;
            }
        }
    }

}

void core_with_non_rt_stuff() {
    printf("[core_with_non_rt_stuff] starting\n");
    Time time;
    InternetManager internet;
    WebServer webServer;

    application.internetManager = &internet;

    internet.init();

    if(internet.connect_to("SSID", "PASSWORD")){
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
    }

    internet.deinit();
}

int main() {
    stdio_init_all();

    for(int i = 5; i >= 0; --i) {
        sleep_ms(1000);
        printf("wait %d...\n", i);
    }

    Storage storage;
    application.storage;

    EffectsManager effectsManager(application);
    application.effectsManager = &effectsManager;

    if(!application.storage->init(Storage::FORMAT)) {
        printf("Failed to init storage \n");
    }
    else {
        storage.makeDir("cfg", false);
    }

    if(!application.effectsManager->init()) {
        printf("Failed to init effectsManager but keep going... \n");
    }
    application.effectsManager->selectEffect(storage.read_uint32_t("cfg/eff_selected"));

    multicore_launch_core1(core_with_python_animations);
    core_with_non_rt_stuff();

    return 0;
}