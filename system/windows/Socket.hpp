#pragma once

#include "Common.hpp"

struct ClientData{};

struct Packet{
    ClientData sender;
    byte payload[1500];
};

class Socket {
public:
    bool sendBroadcast(byte* payload, uint64_t size);
    bool hasPacket() const;
    Packet getPacket();
};