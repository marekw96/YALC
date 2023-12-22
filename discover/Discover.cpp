#include "Discover.hpp"

namespace Discover{

unsigned addToPayload(char* payload, short value){
    payload[0] = (value >> 8) & 0xff;
    payload[1] = value & 0xff;

    return 2;
}

unsigned addToPayload(char* payload, unsigned value){
    payload[0] = (value >> 24) & 0xff;
    payload[1] = (value >> 16) & 0xff;
    payload[2] = (value >> 8) & 0xff;
    payload[3] = value & 0xff;

    return 4;
}

Sender::Sender(DeviceDescription description, SenderSettings settings, SocketFunctions socket)
    :description(description), socket(socket), settings(settings)
{
}

void Sender::periodic(unsigned milisecondsElapsed)
{
    msFromLastSend += milisecondsElapsed;

    if(msFromLastSend > settings.milisecondsBetweenSends) {
        socket.sendBroadcast(socket.obj, data, sizeof(data), settings.port);
        msFromLastSend = 0;
    }
}

void Sender::addTLV(TLVType type, short length, char *buff)
{
    used = addToPayload(&data[used+1], static_cast<unsigned>(type));
    used = addToPayload(&data[used+1], static_cast<short>(length));

    for(int i = 0; i < length; ++i)
        data[used + 1 + i] = buff[i];

    used += length;
}

void Sender::resetTLV()
{
    used = 35;
    for(int i = used; i < sizeof(data) - used; ++i)
        data[1+i] = 0;
}

void Sender::initData()
{
    //Magic values
    data[0] = 'D';
    data[1] = 'S';
    data[2] = 'C';

    //Name
    for(int i = 0; i < sizeof(DeviceDescription::name); ++i)
        data[3+i] = description.name[i];
    data[33] = 0;
    used = 33;

    used += + addToPayload(&data[34],static_cast<short>(description.type));
}

}