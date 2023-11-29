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

    //lwip_nosys_init();
    cyw43_init(&cyw43_state);
    cyw43_cb_tcpip_init(&cyw43_state, CYW43_ITF_STA);



    auto type = app.storage->read_uint32_t("cfg/wifi_mode");
    if(type == 1) {
        prefferedType = NetworkType::STANDALONE;
    }

    printf("[InternetManager]preffered network type: %s\n", to_char(prefferedType));

    storedSSID = app.storage->read_string("cfg/sta_ssid");
    storedPASSWORD = app.storage->read_string("cfg/sta_passwd");
    storedAUTH = static_cast<NetworkAuth>(app.storage->read_uint32_t("cfg/sta_auth"));

    printf("[InternetManager]stored ssid: %s\n", storedSSID.c_str());
    printf("[InternetManager]stored password length: %d\n", storedPASSWORD.size());

    auto apSSIDread = app.storage->read_string("cfg/ap_ssid");
    auto apPASSWORDread = app.storage->read_string("cfg/ap_passwd");

    if(apSSID.size() != 0){
        apSSID = apSSIDread;
        apPASSWORD = apPASSWORDread;
    }
    printf("[InternetManager]AP ssid: %s\n", apSSID.c_str());
    printf("[InternetManager]AP pass: %s\n", apPASSWORD.c_str());


    printf("[InternetManager]init -> done\n");
}

void InternetManager::changedStatusToConnected() {
    printf("[InternetManager]changedStatusToConnected\n");
    connectionStatus = ConnectionStatus::CONNECTED;

    cyw43_cb_tcpip_set_link_up(&cyw43_state, CYW43_ITF_STA);

    cyw43_arch_lwip_begin();
    if(dhcp_start(netif_default) != ERR_OK) {
        printf("Failed to dhcp_start\n");
    } else {
        printf("Started dhcp ?\n");
    }

    cyw43_arch_lwip_end();

    static char mac[18];
    uint8_t bmac[6];
    cyw43_wifi_get_mac(&cyw43_state, CYW43_ITF_STA , bmac);
    sprintf(mac, "%x:%x:%x:%x:%x:%x", bmac[0], bmac[1], bmac[2], bmac[3], bmac[4], bmac[5]);
    printf("MAC %s\n", mac);

    auto status = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);
    printf("status %d\n", status);
    printf("IP: %s\n",ip4addr_ntoa(netif_ip4_addr(netif_default)));
}

void InternetManager::setPrefferedNetworkType(NetworkType type)
{
    printf("[InternetManager]setPrefferedNetworkType %s\n", to_char(type));
    prefferedType = type;
}

ConnectionStatus InternetManager::getStatus()
{
    return connectionStatus;
}

NetworkType InternetManager::getPrefferedNetworkType()
{
    return prefferedType;
}

void InternetManager::storeSTANetowrkData(const std::string& ssid, const std::string& passwd, NetworkAuth auth)
{
    storedAUTH = auth;
    storedSSID = ssid;
    storedPASSWORD = passwd;

    app.storage->store("cfg/sta_ssid", ssid);
    app.storage->store("cfg/sta_passwd", passwd);
    app.storage->store("cfg/sta_auth", static_cast<uint32_t>(storedAUTH));
}

bool InternetManager::connectToSTA()
{
    printf("[InternetManager]connectToSTA\n");
    cyw43_arch_enable_sta_mode();
    return cyw43_arch_wifi_connect_async(storedSSID.c_str(), storedPASSWORD.c_str(), static_cast<uint32_t>(storedAUTH)) == 0;
}

void InternetManager::enableAPMode()
{
    printf("[InternetManager]Enabling AP mode\n");
    cyw43_arch_enable_ap_mode(apSSID.c_str(), apPASSWORD.c_str(), CYW43_AUTH_WPA2_MIXED_PSK);
}

void InternetManager::deinit()
{
    cyw43_arch_deinit();
}

void InternetManager::periodic()
{
    cyw43_arch_poll();
    dhcp_supplied_address(netif_default);
    sys_check_timeouts();

    auto status = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);
    if(status == 3 && connectionStatus != ConnectionStatus::CONNECTED){
        changedStatusToConnected();
    }
}
