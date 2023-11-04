#pragma once

#include "Common.hpp"
#include "messages/Discover.hpp"
#include "messages/MessageId.hpp"
#include "messages/Header.hpp"
#include "messages/SetColors8Bit.hpp"

namespace YALC {
enum class NodeState {NOT_CONNECTED, CONNECTED};

template <typename TimeProvider, typename InputPackets, typename OutputPackets, typename Display>
class DisplayNodeController {
    public:
        DisplayNodeController(TimeProvider& timeProvider, InputPackets& inputPackets, OutputPackets& outputPackets, Display& display)
            : timeProvider(timeProvider), inputPackets(inputPackets), outputPackets(outputPackets), display(display),
              state(NodeState::NOT_CONNECTED),
              lastSendDiscoverTimePoint(0)
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
                handleIncomingPacketsWhenConnected();
            }
        }



    private:
        TimeProvider& timeProvider;
        InputPackets& inputPackets;
        OutputPackets& outputPackets;
        Display& display;
        NodeState state;
        ClientData masterDate;

        TimePoint lastSendDiscoverTimePoint;

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

        void handleIncomingPacketsWhenNotConnected(){
            if(inputPackets.hasPacket()){
                auto packet = inputPackets.getPacket();
                reader::Header header(packet.payload);

                if(header.messageId() == YALC::MessageId::RESPONSE){
                    state = NodeState::CONNECTED;
                    masterDate = packet.sender;
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