#include <stdio.h>
#include "pico/stdlib.h"

#include "Time.hpp"
#include "ws2812/ws2812.hpp"
#include "ws2812/LedString_ws2812.hpp"
#include "../pythonAnimations/AnimationEngine.hpp"
#include "../pythonAnimations/MyAnimation.py.hpp"
#include "InternetManager.hpp"

int main() {
    stdio_init_all();

    Time time;
    LedString_ws2812 ledString(1, 100);
    InternetManager internet;

    auto* vm = new AnimationEngine(ledString);
    for(int i = 5; i > 0; --i){
        printf("wait %d\n", i);
        sleep_ms(1000);
    }

    internet.init();
    if(internet.connect_to("test", "pass")){
        printf("Connected to wifi\n");
    }
    else {
        printf("Unable to connect\n");
    }
    vm->init();
    vm->createAnimation(MyAnimation_py);

    auto current = time.current();
    while(true) {
        ledString.update();
        auto diff = time.current() - current;
        vm->periodic(diff);
        current = time.current();
    }

    internet.deinit();

    delete vm;
    return 0;
}