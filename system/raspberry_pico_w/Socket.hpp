#pragma once

#include "Common.hpp"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

struct ClientData{
    ip_addr_t ipAddr;
    uint16_t port;
};

struct Packet{
    ClientData sender;
    byte payload[1500];
};

class Socket {
public:
    Socket(uint16_t port);
    Socket(const Socket& other) = delete;
    Socket& operator=(const Socket& other) = delete;
    ~Socket();

    bool sendBroadcast(byte* payload, uint64_t size);
    bool send(const ClientData& client, byte* payload, uint64_t size);
    bool hasPacket();
    Packet getPacket();
    void gotPacket(pbuf*p, const ip_addr_t* addr, uint16_t port);

private:
    int socketFd;
    int port;
    udp_pcb *network_pcb;

    bool hasPacketCaputed = false;
    Packet capturedPacket;
};