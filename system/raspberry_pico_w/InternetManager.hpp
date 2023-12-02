#pragma once

#include "lwip/netif.h"
#include "Application.hpp"
#include "pico/cyw43_arch.h"
#include "Time.hpp"

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

static const char* to_char(NetworkAuth auth) {
    switch(auth) {
        case NetworkAuth::NONE:
            return "Open";
        case NetworkAuth::WPA:
            return "WPA";
        case NetworkAuth::WPA2_AES:
            return "WPA2 AES";
        case NetworkAuth::WPA2_MIXED:
            return "WPA2 MIXED";
        default:
            return "?";
    }
}

class InternetManager {
public:
    InternetManager(Application& app);
    void init();
    void storeSTANetowrkData(const std::string& ssid, const std::string& passwd, NetworkAuth auth);
    void storeAPData(const std::string& ssid, const std::string& passwd, NetworkAuth auth);
    bool connectToSTA();
    void enableAPMode();
    void deinit();
    void periodic();
    void changedStatusToConnected();
    void setPrefferedNetworkType(NetworkType type);
    void changeNetworkType(NetworkType type);

    ConnectionStatus getStatus();
    NetworkType getPrefferedNetworkType();

    const std::string& getSTAName();
    NetworkAuth getAuth();
    const std::string& getAPName();
    const std::string& getAPPassword();
    NetworkAuth getAPPAuth();
private:
    Application app;
    netif _netif;
    NetworkType prefferedType = NetworkType::ACCESS_POINT;
    NetworkType requestedNetworkType = NetworkType::ACCESS_POINT;

    std::string storedSSID = "";
    std::string storedPASSWORD = "";
    NetworkAuth storedAUTH = NetworkAuth::NONE;
    TimePoint whenStartedConnectingToSTA{0};

    std::string apSSID = "YALC";
    std::string apPASSWORD = "12345678";
    NetworkAuth apAUTH = NetworkAuth::NONE;

    ConnectionStatus connectionStatus = ConnectionStatus::NOT_CONNECTED;
};