#include "InternetManager.hpp"

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

void InternetManager::init()
{
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
    }
}

bool InternetManager::connect_to(const char *ssid, const char *passwd)
{
    cyw43_arch_enable_sta_mode();
    return cyw43_arch_wifi_connect_timeout_ms(ssid, passwd, CYW43_AUTH_WPA_TKIP_PSK, 30000);
}

void InternetManager::deinit()
{
    cyw43_arch_deinit();
}
