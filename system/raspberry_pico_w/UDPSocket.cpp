#include "UDPSocket.hpp"
#include <cstring>

UDPSocket::UDPSocket() {
    socket = udp_new();
}

UDPSocket::~UDPSocket()
{
    udp_remove(socket);
}

bool UDPSocket::sendBroadcast(void *buffer, unsigned size, short port)
{
    const ip_addr_t broadcast{0xffffffffUL};
    pbuf* pp = pbuf_alloc(PBUF_TRANSPORT, size, PBUF_POOL);
	if (pp == NULL){
        printf("[USPSocket]sendBroadcast failed to init pbuf\n");
        return false;
    }

    memcpy(pp->payload, buffer, size);

    auto result = udp_sendto(socket, pp, &broadcast, port);
    return result == ERR_OK;
}