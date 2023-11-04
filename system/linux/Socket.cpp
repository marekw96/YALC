#include "Socket.hpp"

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

Socket::Socket(uint16_t port)
    : port(port)
{
    socketFd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socketFd == -1) {
        std::cerr << "Failed to init socket. Exit!" << std::endl;
        exit(1);
    }

    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // if(bind(socketFd,reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
    //     std::cerr << "Failed to bind. Exit!" << std::endl;
    //     exit(1);
    // }

    int broadcastEnable=1;
    if(setsockopt(socketFd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) == -1) {
        std::cerr << "Failed to enable bradcast. Exit!" << std::endl;
        exit(1);
    }

}

Socket::~Socket()
{
    close(socketFd);
}

bool Socket::sendBroadcast(byte* payload, uint64_t size)
{
    sockaddr_in to;
    memset(&to, 0, sizeof(to));
    to.sin_family = AF_INET;
    to.sin_addr.s_addr = inet_addr("255.255.255.255");
    to.sin_port = htons(port);

    auto sendBytes = sendto(socketFd, payload, size, 0, reinterpret_cast<sockaddr*>(&to), sizeof(to));
    if(sendBytes != -1) {
        std::cout << "sendBroadcast size: " << size << ", sendBytes: " << sendBytes << std::endl;
    }
    else {
        std::cout << "Unable to send packet, err:" << std::strerror(errno) << std::endl;
    }

    return false;
}

Packet Socket::getPacket()
{
    hasPacketCaputed = false;
    return capturedPacket;
}

bool Socket::send(const ClientData &client, byte *payload, uint64_t size)
{
    auto sendBytes = sendto(socketFd, payload, size, 0, reinterpret_cast<sockaddr*>(const_cast<sockaddr_in*>(&client.addr)), sizeof(client.addr));
    if(sendBytes != -1) {
        std::cout << "send size: " << size << ", sendBytes: " << sendBytes << std::endl;
    }
    else {
        std::cout << "Unable to send packet, err:" << std::strerror(errno) << std::endl;
    }

    return false;
}

bool Socket::hasPacket()
{
    socklen_t received = sizeof(capturedPacket.sender.addr);
    auto recvBytes = recvfrom(socketFd,
                              capturedPacket.payload,
                              sizeof(capturedPacket.payload),
                              MSG_DONTWAIT,
                              reinterpret_cast<sockaddr*>(&capturedPacket.sender.addr),
                              &received);
    if(recvBytes != -1) {
        hasPacketCaputed = true;
        //std::cout << "received packet " << recvBytes << std::endl;
    }

    return hasPacketCaputed;
}
