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

void InternetManager::init()
{
    printf("InternetManager::init\n");
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
    }

    //lwip_nosys_init();
    cyw43_init(&cyw43_state);
    cyw43_cb_tcpip_init(&cyw43_state, CYW43_ITF_STA);
    printf("InternetManager::init -> done\n");
}

void InternetManager::start(){

    //Whoa
    cyw43_cb_tcpip_set_link_up(&cyw43_state, CYW43_ITF_STA);

    // cyw43_arch_lwip_begin();

    // netif_set_status_callback(netif_default, netif_sta_status_callback);
    // netif_set_link_callback(netif_default, link_sta_status_callback);
    // netif_set_hostname(netif_default, "YALC");

    // //dhcp_init();
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

bool InternetManager::connect_to(const char *ssid, const char *passwd)
{
    printf("InternetManager::connect_to\n");
    cyw43_arch_enable_sta_mode();
    return cyw43_arch_wifi_connect_timeout_ms(ssid, passwd, CYW43_AUTH_WPA2_MIXED_PSK, 30000);
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
}
