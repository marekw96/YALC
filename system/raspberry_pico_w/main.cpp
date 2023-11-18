#include <stdio.h>
#include "pico/stdlib.h"

#include "Time.hpp"
#include "ws2812/ws2812.hpp"
#include "ws2812/LedString_ws2812.hpp"
#include "../pythonAnimations/AnimationEngine.hpp"
#include "../pythonAnimations/MyAnimation.py.hpp"
#include "InternetManager.hpp"
#include "WebServer.hpp"
#include "lwip/init.h"
#include "pico/cyw43_arch.h"

int main() {
    stdio_init_all();

    Time time;
    LedString_ws2812 ledString(1, 100);
    InternetManager internet;
    WebServer webServer;

    auto* vm = new AnimationEngine(ledString);
    for(int i = 5; i > 0; --i){
        printf("-wait %d\n", i);
        sleep_ms(1000);
    }

    internet.init();
    if(internet.connect_to("SSID", "passwd")){
        printf("Unable to connect\n");

    }
    else {
        printf("Connected to wifi\n");
    }

    internet.start();
    vm->init();
    webServer.init(80);

    vm->createAnimation(MyAnimation_py);

    auto current = time.current();
    while(true) {
        ledString.update();
        auto diff = time.current() - current;
        vm->periodic(diff);
        current = time.current();
        internet.periodic();
    }

    internet.deinit();

    delete vm;
    return 0;
}