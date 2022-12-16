#include "socket.hpp"

bool socket::start_on_port(uint16_t port) {
    return this->udp.begin(port) == 0;
}

std::vector<uint8_t> socket::read_packet() {
    auto bytes = this->udp.parsePacket();

    if(bytes != 0) {
        std::vector<uint8_t> packet(bytes, 0);
        this->udp.read(packet.data(), bytes);
        this->udp.flush();
        return packet;
    }

    return {};
}

bool socket::send_packet(const char* host, uint16_t port, uint8_t* data, uint32_t size) {
    for(int i = 0; i < size; ++i)
    {
        Serial.print(data[i]);
        Serial.print(" ");
    }
    Serial.println("");
    this->udp.beginPacket(host, port);
    this->udp.write(data, size);
    return this->udp.endPacket() == 1;
}