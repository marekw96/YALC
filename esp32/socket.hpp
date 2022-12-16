#pragma once
#include <vector>

#include "WiFiUdp.h"

class socket{
public:
    bool start_on_port(uint16_t port);
    std::vector<uint8_t> read_packet();
    bool send_packet(const char* host, uint16_t port, uint8_t* data, uint32_t size);

private:
    WiFiUDP udp;
};