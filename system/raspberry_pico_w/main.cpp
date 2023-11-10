#include <stdio.h>
#include "pico/stdlib.h"

#include "Time.hpp"
#include "ws2812/ws2812.hpp"
#include "ws2812/LedString_ws2812.hpp"

int main() {
    setup_default_uart();

    WS2812 leds(2, 10,  pio0, 0);

    printf("Hello, world!\n");

    Time timeProvider;

    LedString_ws2812 ledString(24, 10);
    return 0;
}