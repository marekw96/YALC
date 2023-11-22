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
#include "pico/multicore.h"


void core_with_python_animations(){
    printf("[core_with_python_animations] starting\n");
    Time time;
    LedString_ws2812 ledString(1, 100);

    auto* vm = new AnimationEngine(ledString);

    vm->init();
    vm->createAnimation(MyAnimation_py);

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

    internet.init();
    if(internet.connect_to("SSID", "passwd")){
        printf("Unable to connect\n");

    }
    else {
        printf("Connected to wifi\n");
    }

    internet.start();
    webServer.init(80);

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

    for(int i = 5; i > 0; --i){
        printf("-wait %d\n", i);
        sleep_ms(1000);
    }

    multicore_launch_core1(core_with_python_animations);
    core_with_non_rt_stuff();

    return 0;
}