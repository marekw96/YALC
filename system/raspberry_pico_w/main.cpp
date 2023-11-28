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
#include "Application.hpp"
#include "EffectsManager.hpp"

#include "webpages/ConnectionSettings.hpp"
#include "webpages/EffectsPage.hpp"

#include "../pythonAnimations/FallingStar.py.hpp"

Application application;

void core_with_non_rt_stuff() {
    Time time;
    InternetManager internet(application);
    WebServer webServer;

    application.internetManager = &internet;

    internet.init();

    if(internet.getPrefferedNetworkType() == NetworkType::STANDALONE) {
        internet.connectToSTA();
    }
    else{
        internet.enableAPMode();
    }
    webServer.init(80);

    ConnectionSettingsPage connectionSettingsPage(application);
    EffectsPage effectsPage(application);
    webServer.registerHandler(connectionSettingsPage.getHandler());
    webServer.registerHandler(effectsPage.getHandler());

    LedString_ws2812 ledString(1, 100);
    AnimationEngine vm(ledString);

    ledString.init();
    vm.init();
    auto code = application.effectsManager->getEffectCode(application.effectsManager->getSelectedEffectId());
    vm.createAnimation(code);
    application.effectsManager->restartedEffects();

    auto current = time.current();
    while(true) {
        auto diff = time.current() - current;
        current = time.current();
        internet.periodic();

        ledString.update();
        vm.periodic(diff);
        if(application.effectsManager->hasEffectChanged()) {
            printf("MAIN changing effect\n");
            code = application.effectsManager->getEffectCode(application.effectsManager->getSelectedEffectId());
            vm.deinit();
            vm.init();
            vm.createAnimation(code);
            application.effectsManager->restartedEffects();
            printf("Main effect restarted\n");
        }
    }

    internet.deinit();
}

void delayStartup() {

    for(int i = 5; i >= 0; --i) {
        sleep_ms(1000);
        printf("wait %d...\n", i);
    }

}

int main() {
    stdio_init_all();

    //delayStartup();

    Storage storage;
    application.storage;

    EffectsManager effectsManager(application);
    application.effectsManager = &effectsManager;

    if(!application.storage->init(Storage::DO_NOT_FORMAT)) {
        printf("Failed to init storage \n");
    }
    else {
        storage.makeDir("cfg");
    }

    if(!application.effectsManager->init()) {
        printf("Failed to init effectsManager but keep going... \n");
    }

    application.effectsManager->selectEffect(storage.read_uint32_t("cfg/eff_selected"));
    core_with_non_rt_stuff();

    return 0;
}