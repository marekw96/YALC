#pragma once

#include "effect_information.hpp"
#include "rgb.hpp"
#include "socket.hpp"

class remote_control {
public:
    effect_information get_info();

    void init(Adafruit_NeoPixel& pixels);
    void periodic(int64_t time_elapsed);
    void deinit();
    bool set_parameter(const String& name, const String& value);
    String get_parameter(const String& name);

private:
    void send_handshake();
    void handle_packets();
    void send_iamalive();

    void handle_handshake_response(const std::vector<uint8_t>& packet);
    void handle_color_8_bit(const std::vector<uint8_t>& packet);
    void disconnect_from_server();

    Adafruit_NeoPixel* pixels;
    int64_t counter;
    bool has_connection = false;
    String server_address = "192.168.0.17";
    uint16_t server_port = 12345;
    socket network;
    uint64_t time_elapsed = 0;
    uint64_t last_server_activity = 0;
    uint8_t node_id = 0;
    uint32_t sequence_i_am_alive = 0;
};