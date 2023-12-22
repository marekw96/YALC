#pragma once

#include "lwip/udp.h"
#include "lwip/api.h"

class UDPSocket {
public:
    UDPSocket();
    ~UDPSocket();
    bool sendBroadcast(void* buffer, unsigned size, short port);

private:
    udp_pcb* socket;
};