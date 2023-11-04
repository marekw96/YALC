#pragma once

#include "Common.hpp"
#include <netinet/in.h>

struct ClientData{
    sockaddr_in addr;
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
    bool hasPacket();
    Packet getPacket();

private:
    int socketFd;
    int port;

    bool hasPacketCaputed = false;
    Packet capturedPacket;
};