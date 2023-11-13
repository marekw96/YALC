#include <stdio.h>
#include "pico/stdlib.h"

#include "Time.hpp"
#include "ws2812/ws2812.hpp"
#include "ws2812/LedString_ws2812.hpp"

int main() {
    stdio_init_all();
    setup_default_uart();

    Time timeProvider;
    LedString_ws2812 ledString(1, 10);

    bool s = false;
    while(true) {
        if(s){
            byte d[] = {0,0, 255,0,255,0};
            ledString.setColors(0, d, 2);
        }
        else{
            byte d[] = {0,255, 0,0,0,255};
            ledString.setColors(0, d, 2);
        }
        ledString.update();
        sleep_ms(100);
        s = !s;
    }

    //
    return 0;
}