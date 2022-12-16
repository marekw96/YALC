#include "remote_control.hpp"

#include "effect_parameter.hpp"
#include "view.hpp"

static effect_parameter params[] = {};

class WriteBuffer {
public:
    WriteBuffer(uint8_t* bytes, uint32_t size)
        : data(bytes), current_bit(0), size(size) {}

    void write(uint8_t byte) {
        *data = byte;
        ++data;
    }

    void write(char byte) {
        *data = byte;
        ++data;
    }

    void write(uint16_t two_bytes) {
        *data = (two_bytes >> 8) & 0xFF;
        *(data+1) = two_bytes & 0xFF;
        data += 2;
    }

    void write(uint32_t four_bytes) {
        *data = (four_bytes >> 24) & 0xFF;
        *(data+1) = (four_bytes >> 16) & 0xFF;
        *(data+2) = (four_bytes >> 8) & 0xFF;
        *(data+3) = four_bytes & 0xFF;
        data += 4;
    }

    template<typename T, size_t arrSize>
    void write(const T (&array)[arrSize]) {
        for(int i = 0; i < arrSize; ++i)
           write(array[i]);
    }

private:
    uint8_t* data;
    uint8_t current_bit;
    uint32_t size;
};


namespace messages {
    enum class MessageID {
        HANDSHAKE = 0x01,
        HANDSHAKE_REPLY = 0x02,
        I_AM_ALIVE = 0x03,
        COLOR_LEDS_8_BIT = 0x11
    };

    enum class ColorCode {
        RED = 0x0,
        GREEN = 0x1,
        BLUE = 0x2
    };

    void write_header(WriteBuffer& buff, uint8_t node_id, MessageID message_id, uint16_t seq_id) {
        buff.write(node_id);
        buff.write(static_cast<uint8_t>(message_id));
        buff.write(seq_id);
    }

    uint8_t color_information(ColorCode first, ColorCode second, ColorCode third, bool warm, bool cold) {
        return (static_cast<uint8_t>(first) << 6) | (static_cast<uint8_t>(second) << 4) | (static_cast<uint8_t>(third) << 2) | (warm << 1) | cold;
    }

    uint16_t read_uint16_t(const unsigned char* arr, uint32_t offset) {
        return arr[offset] << 8 | arr[offset+1];
    }
}

effect_information remote_control::get_info()
{
    effect_information info;
    info.id = "remote_control";
    info.name = "Remote control";
    info.description = "Connect to server and control your LEDs remotely";
    info.parameters = view<effect_parameter>(params, sizeof(params) / sizeof(params[0]));
    info.init = std::bind(&remote_control::init, this, std::placeholders::_1);
    info.periodic = std::bind(&remote_control::periodic, this, std::placeholders::_1);
    info.deinit = std::bind(&remote_control::deinit, this);
    info.set_parameter = std::bind(&remote_control::set_parameter, this, std::placeholders::_1, std::placeholders::_2);
    info.get_parameter = std::bind(&remote_control::get_parameter, this, std::placeholders::_1);

    return info;
}

void remote_control::init(Adafruit_NeoPixel& pixels) {
    Serial.println("remote_control::init");

    this->pixels = &pixels;
    this->pixels->clear();

    this->counter = 0;
    this->has_connection = false;
    this->network.start_on_port(this->server_port);
    this->last_server_activity = 0;
    this->time_elapsed = 0;
    this->node_id = 0;
    this->sequence_i_am_alive = 0;
}

void remote_control::periodic(int64_t time_elapsed) {
    this->counter += time_elapsed;
    this->time_elapsed += time_elapsed;

    if(this->counter > 1000000) {
        if(this->has_connection) {
            this->send_iamalive();
        }
        else {
            this->send_handshake();
        }

        this->counter = 0;
    }

    if(this->has_connection) {
        if(this->time_elapsed - this->last_server_activity > 1500000) {
            this->disconnect_from_server();
        }
    }

    this->handle_packets();
}

void remote_control::disconnect_from_server() {
    this->has_connection = false;
    Serial.println("Disconnected from server due to server inactivity");
}

void remote_control::handle_packets() {
    auto packet = this->network.read_packet();

    if(packet.size() != 0) {
        uint8_t node_id = packet[0];
        messages::MessageID msg_id = static_cast<messages::MessageID>(packet[1]);
        uint16_t sequence_id = messages::read_uint16_t(packet.data(), 2);

        Serial.print("Got packet: node_id: ");
        Serial.print(node_id);
        Serial.print(", msg_id: ");
        Serial.print(static_cast<int>(msg_id));
        Serial.print(", sequence_id: ");
        Serial.println(sequence_id);

        switch(msg_id) {
            case messages::MessageID::HANDSHAKE_REPLY:
                this->handle_handshake_response(packet);
            break;

            case messages::MessageID::COLOR_LEDS_8_BIT:
                this->handle_color_8_bit(packet);
            break;

            default:
                Serial.print("Unable to handle packet");
            break;
        }
    }
}

void remote_control::handle_handshake_response(const std::vector<uint8_t>& packet) {
    if(this->has_connection) {
        return;
    }

    uint32_t reply_device_id = *reinterpret_cast<const uint32_t*>(packet.data() + 4);
    uint32_t device_id = *reinterpret_cast<const uint32_t*>(packet.data() + 8);
    this->node_id = packet[62];

    this->has_connection = true;
    this->last_server_activity = this->time_elapsed;
}

void remote_control::handle_color_8_bit(const std::vector<uint8_t>& packet) {
    if(!this->has_connection) {
        return;
    }

    uint16_t start_leds = messages::read_uint16_t(packet.data(), 4);
    uint16_t number_of_colors = messages::read_uint16_t(packet.data(), 6);
    for(int i = 0; i < number_of_colors; ++i) {
        this->pixels->setPixelColor(i, this->pixels->Color(packet[8+i*3], packet[8+i*3+1], packet[8+i*3+2]));
    }

    this->pixels->show();
    this->last_server_activity = this->time_elapsed;
}

void remote_control::send_handshake() {
    if(this->has_connection) {
        return;
    }

    uint32_t device_id = 0x01;
    uint8_t sw_version[] = {0x00, 0x00, 0x04};
    uint8_t api_version[] = {0x00, 0x00, 0x03};
    uint16_t number_of_leds = this->pixels->numPixels();
    uint8_t rgb_w_w = messages::color_information(messages::ColorCode::GREEN, messages::ColorCode::RED, messages::ColorCode::BLUE, false, false);
    uint16_t mtu = 1500;
    uint8_t leds_per_meter = 10;
    char name[40] = {'Y', 'A', 'L', 'C'};

    uint8_t msg[65] = {0};
    WriteBuffer buff(&msg[0], 65);

    messages::write_header(buff, this->node_id, messages::MessageID::HANDSHAKE, 0);
    buff.write(device_id);
    buff.write(sw_version[0]);
    buff.write(sw_version[1]);
    buff.write(sw_version[2]);
    buff.write(api_version[0]);
    buff.write(api_version[1]);
    buff.write(api_version[2]);
    buff.write(number_of_leds);
    buff.write(rgb_w_w);
    buff.write(mtu);
    buff.write(leds_per_meter);
    buff.write(name);

    this->network.send_packet(this->server_address.c_str(), this->server_port, &msg[0], 65);
}

void remote_control::send_iamalive() {
    if(!this->has_connection) {
        return;
    }

    uint8_t msg[8] = {0};
    WriteBuffer buff(&msg[0], 8);

    messages::write_header(buff, this->node_id, messages::MessageID::I_AM_ALIVE, ++this->sequence_i_am_alive);
    buff.write(static_cast<uint32_t>(this->counter));

    this->network.send_packet(this->server_address.c_str(), this->server_port, &msg[0], 8);
}

bool remote_control::set_parameter(const String& name, const String& value) {
    return false;
}

String remote_control::get_parameter(const String& name) {
    return {};
}

void remote_control::deinit() {
    this->has_connection = false;
}