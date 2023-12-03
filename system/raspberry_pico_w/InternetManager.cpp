#include "InternetManager.hpp"

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/netifapi.h"
#include "lwip/timeouts.h"
#include "lwip/dhcp.h"



static void netif_sta_status_callback(netif *netif)
{
    printf("netif_sta_status_callback %s\n", ip4addr_ntoa(netif_ip4_addr(netif)));
}
static void link_sta_status_callback (struct netif *netif)
{
    printf("link_sta_status_callback %s\n", ip4addr_ntoa(netif_ip4_addr(netif)));
}

InternetManager::InternetManager(Application &app)
    :app(app)
{}

void InternetManager::init()
{
    printf("[InternetManager]init\n");
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
    }

    cyw43_init(&cyw43_state);
    cyw43_cb_tcpip_init(&cyw43_state, CYW43_ITF_STA);

    auto type = app.storage->read_uint32_t("cfg/wifi_mode");
    if(type != 0) {
        prefferedType = NetworkType::STANDALONE;
        requestedNetworkType = prefferedType;
    }

    printf("[InternetManager]preffered network type: %s\n", to_char(prefferedType));

    storedSSID = app.storage->read_string("cfg/sta_ssid");
    storedPASSWORD = app.storage->read_string("cfg/sta_passwd");
    storedAUTH = static_cast<NetworkAuth>(app.storage->read_uint32_t("cfg/sta_auth"));

    printf("[InternetManager]stored ssid: %s\n", storedSSID.c_str());
    //printf("[InternetManager]stored password: %s\n", storedPASSWORD.c_str());
    printf("[InternetManager]stored password length: %d\n", storedPASSWORD.size());
    printf("[InternetManager]stored auth: %s\n", to_char(storedAUTH));

    apSSID = app.storage->read_string("cfg/ap_ssid");
    apPASSWORD = app.storage->read_string("cfg/ap_passwd");
    apAUTH = static_cast<NetworkAuth>(app.storage->read_uint32_t("cfg/ap_auth"));

    printf("[InternetManager]AP ssid: %s\n", apSSID.c_str());
    printf("[InternetManager]AP pass: %s\n", apPASSWORD.c_str());
    printf("[InternetManager]AP auth: %s\n", to_char(apAUTH));

    printf("[InternetManager]init -> done\n");
}

void InternetManager::changedStatusToConnected() {
    printf("[InternetManager]changedStatusToConnected\n");
    connectionStatus = ConnectionStatus::CONNECTED;

    static char mac[18];
    uint8_t bmac[6];
    cyw43_wifi_get_mac(&cyw43_state, CYW43_ITF_STA , bmac);
    sprintf(mac, "%x:%x:%x:%x:%x:%x", bmac[0], bmac[1], bmac[2], bmac[3], bmac[4], bmac[5]);
    printf("[InternetManager]MAC %s\n", mac);

    auto status = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);
    printf("[InternetManager]status %d\n", status);
    printf("[InternetManager]IP: %s\n",ip4addr_ntoa(netif_ip4_addr(netif_default)));

    auto link_status = cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA);
    printf("[InternetManager]link status %d\n", link_status);
}

void InternetManager::setPrefferedNetworkType(NetworkType type)
{
    printf("[InternetManager]setPrefferedNetworkType %s\n", to_char(type));
    prefferedType = type;
}

void InternetManager::changeNetworkType(NetworkType type)
{
    requestedNetworkType = type;
    connectionStatus = ConnectionStatus::NOT_CONNECTED;

    if(type == NetworkType::ACCESS_POINT){
        app.storage->store("cfg/wifi_mode", 0u);
    }
    else if(type == NetworkType::STANDALONE) {
        app.storage->store("cfg/wifi_mode", 1u);
        whenStartedConnectingToSTA = Time{}.current();
    }
}

ConnectionStatus InternetManager::getStatus()
{
    return connectionStatus;
}

NetworkType InternetManager::getPrefferedNetworkType()
{
    return prefferedType;
}

const std::string& InternetManager::getSTAName(){
    return storedSSID;
}

NetworkAuth InternetManager::getAuth()
{
    return storedAUTH;
}

const std::string& InternetManager::getAPName()
{
    return apSSID;
}

const std::string& InternetManager::getAPPassword()
{
    return apPASSWORD;
}

NetworkAuth InternetManager::getAPPAuth()
{
    return apAUTH;
}

void InternetManager::startDHCP()
{
    cyw43_cb_tcpip_set_link_up(&cyw43_state, CYW43_ITF_STA);

    cyw43_arch_lwip_begin();

    if(dhcp_start(netif_default) != ERR_OK) {
        printf("[InternetManager]Failed to dhcp_start\n");
    } else {
        printf("[InternetManager]Started dhcp ?\n");
    }

    cyw43_arch_lwip_end();

    static char mac[18];
    uint8_t bmac[6];
    cyw43_wifi_get_mac(&cyw43_state, CYW43_ITF_STA , bmac);
    sprintf(mac, "%x:%x:%x:%x:%x:%x", bmac[0], bmac[1], bmac[2], bmac[3], bmac[4], bmac[5]);
    printf("[InternetManager]MAC %s\n", mac);

    auto status = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);
    printf("[InternetManager]status %d\n", status);
    printf("[InternetManager]IP: %s\n",ip4addr_ntoa(netif_ip4_addr(netif_default)));


    auto link_status = cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA);
    printf("[InternetManager]link status %d\n", link_status);

    isDHCPStarted = true;
}

void InternetManager::storeSTANetowrkData(const std::string& ssid, const std::string& passwd, NetworkAuth auth)
{
    storedAUTH = auth;
    storedSSID = ssid;
    storedPASSWORD = passwd;

    app.storage->store("cfg/sta_ssid", storedSSID);
    app.storage->store("cfg/sta_passwd", storedPASSWORD);
    app.storage->store("cfg/sta_auth", static_cast<uint32_t>(storedAUTH));
}

void InternetManager::storeAPData(const std::string &ssid, const std::string &passwd, NetworkAuth auth)
{
    apAUTH = auth;
    apSSID = ssid;
    apPASSWORD = passwd;

    app.storage->store("cfg/ap_ssid", ssid);
    app.storage->store("cfg/ap_passwd", passwd);
    app.storage->store("cfg/ap_auth", static_cast<uint32_t>(auth));
}

bool InternetManager::connectToSTA()
{
    printf("[InternetManager]connectToSTA\n");
    connectionStatus = ConnectionStatus::NOT_CONNECTED;
    prefferedType = NetworkType::STANDALONE;
    requestedNetworkType = NetworkType::STANDALONE;

    cyw43_arch_disable_ap_mode();

    dhcp_release(netif_default);
    dhcp_stop(netif_default);
    isDHCPStarted = false;

    cyw43_arch_enable_sta_mode();
    whenStartedConnectingToSTA = Time{}.current();
    printf("[InternetManager::connectToSTA] %s %s %s\n", storedSSID.c_str(),storedPASSWORD.c_str(), to_char(storedAUTH));
    auto res = cyw43_arch_wifi_connect_async(storedSSID.c_str(), storedPASSWORD.c_str(), static_cast<uint32_t>(storedAUTH)) == 0;

    return res;
}

void InternetManager::enableAPMode()
{
    printf("[InternetManager]Enabling AP mode\n");
    connectionStatus = ConnectionStatus::NOT_CONNECTED;
    prefferedType = NetworkType::ACCESS_POINT;
    requestedNetworkType = NetworkType::ACCESS_POINT;

    cyw43_arch_disable_sta_mode();
    cyw43_arch_enable_ap_mode(apSSID.c_str(), apPASSWORD.c_str(), static_cast<uint32_t>(apAUTH));

    //it's already connected to AP
    changedStatusToConnected();
}

void InternetManager::deinit()
{
    cyw43_arch_deinit();
}
static int last_status = 0;
void InternetManager::periodic()
{
    cyw43_arch_poll();
    sys_check_timeouts();

    auto status = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);
    if(last_status != status) {
        printf("Status changed from %d to %d \n", last_status, status);
        last_status = status;
    }

    if(status == 1 && isDHCPStarted == false) {
        startDHCP();
    }
    if(status == 3 && connectionStatus != ConnectionStatus::CONNECTED){
        changedStatusToConnected();
    }

    if(connectionStatus != ConnectionStatus::CONNECTED){
        auto now = Time{}.current();
        auto diff = now - whenStartedConnectingToSTA;
        if(diff.asSeconds() > 10) {
            printf("[InternetManager]Still not connected after 10 secs. Starting AP\n");
            printf("[InternetManager]diff %d\n", diff.asSeconds());
            printf("[InternetManager]status %d\n", status);
            enableAPMode();
        }
    }

    if(requestedNetworkType != prefferedType) {
        //setPrefferedNetworkType(requestedNetworkType);
        if(requestedNetworkType == NetworkType::ACCESS_POINT) {
            enableAPMode();
        }
        else if(requestedNetworkType == NetworkType::STANDALONE){
            connectToSTA();
        }
    }
}
