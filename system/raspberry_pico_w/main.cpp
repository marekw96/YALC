#include <stdio.h>
#include "pico/stdlib.h"

#include "Time.hpp"
#include "ws2812/ws2812.hpp"
#include "ws2812/LedString_ws2812.hpp"
#include "../pythonAnimations/AnimationEngine.hpp"
#include "../pythonAnimations/MyAnimation.py.hpp"

int main() {
    stdio_init_all();

    Time time;
    LedString_ws2812 ledString(1, 100);

    auto* vm = new AnimationEngine(ledString);
    vm->createAnimation(MyAnimation_py);

    set(ledString, 255, 0,0);

    auto current = time.current();
    while(true) {
        ledString.update();
        auto diff = time.current() - current;
        vm->periodic(diff);
        current = time.current();
    }


    delete vm;
    return 0;
}