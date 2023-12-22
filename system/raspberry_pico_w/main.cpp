#include <stdio.h>
#include "pico/stdlib.h"

#include "Time.hpp"
#include "ws2812/ws2812.hpp"
#include "ws2812/LedString_ws2812.hpp"
#include "ws2812/LedStringCombined.hpp"
#include "../pythonAnimations/AnimationEngine.hpp"

#include "InternetManager.hpp"
#include "WebServer.hpp"
#include "lwip/init.h"
#include "pico/cyw43_arch.h"
#include "Application.hpp"
#include "EffectsManager.hpp"
#include "LedsConfiguration.hpp"

#include "webpages/ConnectionSettings.hpp"
#include "webpages/EffectsPage.hpp"
#include "webpages/IndexPage.hpp"
#include "webpages/LedsConfigurationPage.hpp"
#include "webpages/RebootPage.hpp"

#include "UDPSocket.hpp"
#include "Discover.hpp"

#include "../pythonAnimations/FallingStar.py.hpp"

Application application;

void core_with_non_rt_stuff() {
    Time time;
    InternetManager internet(application);
    WebServer webServer;
    LedsConfiguration ledsConfiguration(application);
    UDPSocket udpSocket;
    Discover::DeviceDescription description{.name = {'L', 'E','D', 'Y'}, .type = Discover::Type::LEDS};
    Discover::SocketFunctions socketBroadcastFunctions;
    socketBroadcastFunctions.obj = &udpSocket;
    socketBroadcastFunctions.sendBroadcast = reinterpret_cast<Discover::SendUDPPacketFunctionType>(&UDPSocket::sendBroadcast);
    //Discover::Sender discoverMessanger(description, {}, socketBroadcastFunctions);

    application.internetManager = &internet;
    application.ledsConfiguration = &ledsConfiguration;

    internet.init();
    ledsConfiguration.init();

    if(internet.getPrefferedNetworkType() == NetworkType::STANDALONE) {
        internet.connectToSTA();
    }
    else{
        internet.enableAPMode();
    }
    webServer.init(80);

    ConnectionSettingsPage connectionSettingsPage(application);
    EffectsPage effectsPage(application);
    IndexPage indexPage(application);
    LedsConfigurationPage ledsConfigurationPage(application);
    RebootPage rebootPage(application);
    webServer.registerHandler(connectionSettingsPage.getHandler());
    webServer.registerHandler(effectsPage.getHandler());
    webServer.registerHandler(ledsConfigurationPage.getHandler());
    webServer.registerHandler(rebootPage.getHandler());
    webServer.registerHandler(indexPage.getHandler());

    LedString_ws2812 ledString1(0, ledsConfiguration.getPixelsFor(0));
    LedString_ws2812 ledString2(1, ledsConfiguration.getPixelsFor(1));
    LedStringCombined ledStrings(ledString1, ledString2);
    AnimationEngine vm(ledStrings);
    application.animationEngine = &vm;

    ledStrings.init();

    auto code = application.effectsManager->getEffectCode(application.effectsManager->getSelectedEffectId());
    vm.changeAnimation(code);

    application.effectsManager->restartedEffects();

    auto current = time.current();
    while(true) {
        auto diff = time.current() - current;
        current = time.current();
        internet.periodic();

        ledStrings.update();
        vm.periodic(diff);
        //discoverMessanger.periodic(diff.asMiliseconds());
        if(application.effectsManager->hasEffectChanged()) {
            printf("MAIN changing effect\n");
            code = application.effectsManager->getEffectCode(application.effectsManager->getSelectedEffectId());
            vm.changeAnimation(code);
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
        if(application.storage->init(Storage::FORMAT)) {
            printf("Formated ok!\n");
        }
        else{
            printf("Failed to format again!!");
            delayStartup();
            printf("Failed to format again!!");
        }
    }

    if(storage.read_uint32_t("cfg/init") != 1) {
        storage.makeDir("cfg");
        storage.store("cfg/wifi_mode", 0u);
        storage.store("cfg/ap_ssid", std::string("YALC"));
        storage.store("cfg/ap_passwd", std::string("12345678"));
        storage.store("cfg/ap_passwd", std::string("12345678"));
        storage.store("cfg/leds", "0\n100");
        storage.store("cfg/init", 1);
    }

    if(!application.effectsManager->init()) {
        printf("Failed to init effectsManager but keep going... \n");
    }

    application.effectsManager->selectEffect(storage.read_uint32_t("cfg/eff_selected"));
    core_with_non_rt_stuff();

    return 0;
}