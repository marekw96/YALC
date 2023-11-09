#include <stdio.h>
#include "pico/stdlib.h"

#include "Time.hpp"
#include "ws2812/ws2812.hpp"

int main() {
    setup_default_uart();
    WS2812_Init();


    printf("Hello, world!\n");

    Time timeProvider;
    return 0;
}