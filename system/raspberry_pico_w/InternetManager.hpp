#pragma once

#include "lwip/netif.h"

class InternetManager {
public:
    void init();
    bool connect_to(const char* ssid, const char* passwd);
    void deinit();
    void periodic();
    void start();

private:
    netif _netif;
};