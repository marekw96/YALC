#include <stdio.h>
#include "pico/stdlib.h"

#include "Time.hpp"
#include "ws2812/ws2812.hpp"
#include "ws2812/LedString_ws2812.hpp"
#include "../pythonAnimations/AnimationEngine.hpp"
#include "../pythonAnimations/MyAnimation.py.hpp"

void set(LedString_ws2812& led, byte r, byte g, byte b){
    byte d[] = {g,r,b};
    led.setColors(0, d, 1);
    led.update();
    sleep_ms(500);
}


int main() {
    stdio_init_all();



    Time time;
    LedString_ws2812 ledString(1, 100);
    set(ledString, 0, 255,255);
    for(int i = 10; i > 0; --i){
        sleep_ms(1000);
        printf("wait %d\n", i);
    }

    auto* vm = new AnimationEngine(ledString); printf("AnimationEngine\n");
    set(ledString, 0, 255,0);
    vm->init(); printf("vm->init(); \n");
    set(ledString, 255, 255,0);
    vm->createAnimation(MyAnimation_py); printf("vm->createAnimation(MyAnimation_py); \n");

    set(ledString, 255, 0,0);

    auto current = time.current();
    while(true) { printf("while(true) { \n");
        ledString.update();
        auto diff = time.current() - current;
        vm->periodic(diff);
        current = time.current();
    }


    delete vm;
    return 0;
}