#include "Socket.hpp"

#include <iostream>

bool Socket::sendBroadcast(byte* payload, uint64_t size)
{
    std::cout << "sendBroadcast size: " << size << std::endl;
    return false;
}

Packet Socket::getPacket()
{
    return Packet();
}

bool Socket::hasPacket() const
{
    return false;
}