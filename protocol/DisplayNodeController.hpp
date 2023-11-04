#pragma once

#include "Common.hpp"
#include "messages/Discover.hpp"
#include "messages/MessageId.hpp"
#include "messages/Header.hpp"
#include "messages/SetColors8Bit.hpp"
#include "messages/Ping.hpp"

namespace YALC {
enum class NodeState {NOT_CONNECTED, CONNECTED};

template <typename TimeProvider, typename InputPackets, typename OutputPackets, typename Display>
class DisplayNodeController {
    public:
        DisplayNodeController(TimeProvider& timeProvider, InputPackets& inputPackets, OutputPackets& outputPackets, Display& display)
            : timeProvider(timeProvider), inputPackets(inputPackets), outputPackets(outputPackets), display(display),
              state(NodeState::NOT_CONNECTED),
              lastSendDiscoverTimePoint(0), lastSendPingTimePoint(0)
        {}

        void periodic() {
            auto currentTime = timeProvider.current();

            if(state == NodeState::NOT_CONNECTED) {
                auto timeElapsed = currentTime - lastSendDiscoverTimePoint;

                if(timeElapsed.asSeconds() > 5) {
                    sendDiscover();
                    lastSendDiscoverTimePoint = currentTime;
                }

                handleIncomingPacketsWhenNotConnected();
            }
            else {
                auto timeElapsedPing = currentTime - lastSendPingTimePoint;
                if(timeElapsedPing.asSeconds() > 1) {
                    sendPing();
                    lastSendPingTimePoint = currentTime;
                }

                handleIncomingPacketsWhenConnected();
            }
        }



    private:
        TimeProvider& timeProvider;
        InputPackets& inputPackets;
        OutputPackets& outputPackets;
        Display& display;
        NodeState state;
        ClientData masterData;

        TimePoint lastSendDiscoverTimePoint;
        TimePoint lastSendPingTimePoint;

        uint32_t pingCounter;

        void sendDiscover() {
            byte packetData[1500] = {};
            writer::Discover packet(packetData);
            packet.header().nodeId(0)
                           .messageId(MessageId::DISCOVER)
                           .sequenceId(0);

            packet.deviceId(0x1)
                  .swMajor(0)
                  .swMinor(1)
                  .swPatch(0)
                  .apiMajor(1)
                  .apiMinor(0)
                  .apiPatch(0)
                  .ledsCount(display.ledsCount())
                  .colorOrder(display.colorOrder())
                  .naturalWhite(false)
                  .warmWhite(false)
                  .coldWhite(false)
                  .MTU(1500)
                  .ledsPerMeter(30)
                  .name("YALC - slave node");

            outputPackets.sendBroadcast(packetData, packet.size());
        }

        void sendPing() {
            byte packetData[1500] = {};
            writer::Ping packet(packetData);
            packet.header().nodeId(0)
                           .messageId(MessageId::PING)
                           .sequenceId(0);

            packet.sequence(pingCounter);

            outputPackets.send(masterData, packetData, packet.size());
        }

        void handleIncomingPacketsWhenNotConnected(){
            if(inputPackets.hasPacket()){
                auto packet = inputPackets.getPacket();
                reader::Header header(packet.payload);

                if(header.messageId() == YALC::MessageId::RESPONSE){
                    state = NodeState::CONNECTED;
                    masterData = packet.sender;
                    lastSendPingTimePoint = timeProvider.current();
                    pingCounter = 0;
                }
            }
        }

        void handleIncomingPacketsWhenConnected() {
            if(inputPackets.hasPacket()){
                auto packet = inputPackets.getPacket();
                reader::Header header(packet.payload);

                if(header.messageId() == YALC::MessageId::SET_COLORS_8BIT) {
                    handleSetPixels(packet);
                }
            }
        }

        void handleSetPixels(const Packet& packet) {
            reader::SetColors8Bit pixels(packet.payload);

            //std::cout << "handleSetPixels| start " << pixels.startPixel() << ", numberOfPixles " << pixels.numberOfPixels() << std::endl;

            display.setColors(pixels.startPixel(),
                              reinterpret_cast<byte*>(pixels.getPixelsArray()),
                              pixels.numberOfPixels());
        }
};
}