#pragma once

#include "lwip/netif.h"

enum class ConnectionStatus {
    CONNECTED,
    NOT_CONNECTED
};

static const char* to_char(ConnectionStatus status) {
    switch(status) {
        case ConnectionStatus::CONNECTED:
            return "Connected";
        case ConnectionStatus::NOT_CONNECTED:
            return "Not connected";
        default:
            return "?";
    }
}

enum class NetworkType {
    STANDALONE,
    ACCESS_POINT
};

class InternetManager {
public:
    void init();
    bool connect_to(const char* ssid, const char* passwd);
    void deinit();
    void periodic();
    void start();

    ConnectionStatus getStatus();

private:
    netif _netif;
};