#pragma once

/* Sends packet to broadcast (by default) on specified port (7744)
 * to allow identify device in network. Contains information about type
 * and other value contained in TLV(type, length, value) structure.
 *
 * Packet format is:
 * [0] - 'D'
 * [1] - 'S'
 * [2] - 'C'
 * [3-32] - device name in ASCII, should be null terminated
 * [33] - reserved. Should be 0. In case name was not null terminated\
 * [34-36] - type, defined in Discover::Type
 * [37-1500] TLV defined as below
 *
 * TLV :
 * [0-3] - TLV type defined in TLVType
 * [4-5] - size in bytes
 * [6-6+size] - payload, should be interpreted by client
 */

namespace Discover {

enum class Type : short {
    LEDS = 1,
    CHRISTMAS_TREE = 2,
};

using SendUDPPacketFunctionType = bool(*)(void*, char*, unsigned, short port);

struct SocketFunctions {
    void* obj;
    SendUDPPacketFunctionType sendBroadcast;
};

enum class TLVType : unsigned {
    WebControlAddress = 1,
    NumberOfLeds = 2,
    LedType = 3
};

struct DeviceDescription {
    char name[32] = {0};
    Type type;
};

struct SenderSettings {
    short port = 7744;
    unsigned milisecondsBetweenSends = 1000;
};

class Sender {
public:
    Sender(DeviceDescription description, SenderSettings settings, SocketFunctions socket);

    void periodic(unsigned milisecondsElapsed);
    void addTLV(TLVType type, short length, char* data);
    void resetTLV();

private:
    SocketFunctions socket;
    DeviceDescription description;
    SenderSettings settings;
    char data[1500] = {0};
    unsigned used = 0;
    unsigned msFromLastSend = 0;

    void initData();
};

}