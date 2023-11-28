#pragma once

#include "lwip/netif.h"
#include "Application.hpp"
#include "pico/cyw43_arch.h"

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
    STANDALONE = 1,
    ACCESS_POINT = 0
};

static const char* to_char(NetworkType type) {
    switch(type) {
        case NetworkType::ACCESS_POINT:
            return "Access Point";
        case NetworkType::STANDALONE:
            return "Standalone";
        default:
            return "?";
    }
}

enum class NetworkAuth {
    NONE = CYW43_AUTH_OPEN,
    WPA = CYW43_AUTH_WPA_TKIP_PSK,
    WPA2_AES = CYW43_AUTH_WPA2_AES_PSK,
    WPA2_MIXED = CYW43_AUTH_WPA2_MIXED_PSK
};

class InternetManager {
public:
    InternetManager(Application& app);
    void init();
    void storeSTANetowrkData(const std::string& ssid, const std::string& passwd, NetworkAuth auth);
    bool connectToSTA();
    void enableAPMode();
    void deinit();
    void periodic();
    void changedStatusToConnected();
    void setPrefferedNetworkType(NetworkType type);

    ConnectionStatus getStatus();
    NetworkType getPrefferedNetworkType();

private:
    Application app;
    netif _netif;
    NetworkType prefferedType = NetworkType::STANDALONE;
    std::string storedSSID = "";
    std::string storedPASSWORD = "";
    NetworkAuth storedAUTH = NetworkAuth::NONE;
    std::string apSSID = "YALC";
    std::string apPASSWORD = "12345678";
    ConnectionStatus connectionStatus = ConnectionStatus::NOT_CONNECTED;
};