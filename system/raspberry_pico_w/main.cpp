#include <stdio.h>
#include "pico/stdlib.h"

#include "Time.hpp"

int main() {
    setup_default_uart();
    printf("Hello, world!\n");

    Time timeProvider;
    return 0;
}