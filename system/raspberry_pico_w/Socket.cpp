#include "Socket.hpp"

void handleRecvFreeFunc(void* arg, udp_pcb* pcb, pbuf* p, const ip_addr_t* addr, uint16_t port) {
    Socket* socket = reinterpret_cast<Socket*>(arg);
    socket->gotPacket(p, addr, port);
}

Socket::Socket(uint16_t port)
    : port(port)
{
    if (cyw43_arch_init()) {
        printf("cyw43_arch_init - failed to initialise\n");
    }

    network_pcb = udp_new_ip_type(IPADDR_TYPE_ANY);
    if(!network_pcb) {
        printf("udp_new_ip_type(IPADDR_TYPE_ANY) - failed to create\n");
    }

    udp_recv(network_pcb, handleRecvFreeFunc, this);
}

Socket::~Socket()
{
    cyw43_arch_deinit();
}

bool Socket::sendBroadcast(byte* payload, uint64_t size)
{
    cyw43_arch_lwip_begin();
    ip4_addr broadcastIp;
    ipaddr_aton("255.255.255.255", &broadcastIp);

    pbuf *p = pbuf_alloc(PBUF_TRANSPORT, size, PBUF_RAM);
    pbuf_take(p, payload, size);

    udp_sendto(network_pcb, p, &broadcastIp, port);

    pbuf_free(p);
    cyw43_arch_lwip_end();

    return false;
}

Packet Socket::getPacket()
{
    hasPacketCaputed = false;
    return capturedPacket;
}

bool Socket::send(const ClientData &client, byte *payload, uint64_t size)
{
    cyw43_arch_lwip_begin();
    pbuf *p = pbuf_alloc(PBUF_TRANSPORT, size, PBUF_RAM);
    pbuf_take(p, payload, size);

    udp_sendto(network_pcb, p, &client.ipAddr, client.port);

    pbuf_free(p);
    cyw43_arch_lwip_end();
    return false;
}

bool Socket::hasPacket()
{
    return hasPacketCaputed;
}

void Socket::gotPacket(pbuf *p, const ip_addr_t *addr, uint16_t port)
{
    //For now only one packet is queued
    capturedPacket.sender.port = port;
    ip_addr_copy(capturedPacket.sender.ipAddr, *addr);
    pbuf_copy_partial(p, capturedPacket.payload, p->tot_len, 0);
    hasPacketCaputed = true;
}
