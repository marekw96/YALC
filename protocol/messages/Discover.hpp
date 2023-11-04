#pragma once

#include "Common.hpp"
#include "Header.hpp"
#include "MessageId.hpp"

/*
 header Header 4 bytes YALC::Header
 device_id 4 bytes unsigned
 sw_major 1 byte unsigned
 sw_minor 1 byte unsigned
 sw_patch 1 byte unsigned
 api_major 1 byte unsigned
 api_minor 1 byte unsigned
 api_patch 1 byte unsigned
 leds_count 4 bytes unsigned
 color_order 5 bits ColorOrder
 natural_white 1 bit bool
 warm_white 1 bit bool
 cold_white 1 bit bool
 MTU 2 bytes unsigned
 leds_per_meter 1 byte unsigned
 name 40 bytes
*/

namespace YALC {
    struct DiscoverName { char name[40] = {'0'}; };

    namespace reader {
        class Discover {
            public:
                Discover(byte* const ptr) : data(ptr) {}
                Header header() const { return Header(data);}
                uint32_t deviceId() const { return *reinterpret_cast<uint32_t*>(data+4); }
                byte swMajor() const { return data[8]; }
                byte swMinor() const { return data[9]; }
                byte swPatch() const { return data[10]; }
                byte apiMajor() const { return data[11]; }
                byte apiMinor() const { return data[12]; }
                byte apiPatch() const { return data[13]; }
                uint32_t ledsCount() const { return *reinterpret_cast<uint32_t*>(data+14); }
                ColorOrder colorOrder() const { return static_cast<ColorOrder>((data[18] >> 3) & 0b11111); }
                bool naturalWhite() const { return (data[18] >> 2) & 0b1; }
                bool warmWhite() const { return (data[18] >> 1) & 0b1; }
                bool coldWhite() const { return data[18] & 0b1; }
                uint16_t MTU() const { return *reinterpret_cast<uint16_t*>(data+19); }
                byte ledsPerMeter() const { return data[20]; }
                DiscoverName name() const { DiscoverName name; for(int id = 0; id < 40 && data[21 + id] != '0'; ++id) name.name[data[21+id]]; return name;}
            private:
                byte* const data;
        };
    }

    namespace writer {
        class Discover {
            public:
                Discover(byte* const ptr) : data(ptr) {}
                Header header() { return Header(data); }
                Discover& deviceId(uint32_t value) { *reinterpret_cast<uint32_t*>(data + 4) = value; return *this; }
                Discover& swMajor(byte value) { data[8] = value; return *this; }
                Discover& swMinor(byte value) { data[9] = value; return *this; }
                Discover& swPatch(byte value) { data[10] = value; return *this; }
                Discover& apiMajor(byte value) { data[11] = value; return *this; }
                Discover& apiMinor(byte value) { data[12] = value; return *this; }
                Discover& apiPatch(byte value) { data[13] = value; return *this; }
                Discover& ledsCount(uint32_t value) { *reinterpret_cast<uint32_t*>(data + 14) = value; return *this; }
                Discover& colorOrder(ColorOrder value) {data[18] = (data[18] & 0b00011111) | (static_cast<byte>(value) << 3); return *this; }
                Discover& naturalWhite(bool value) {data[18] = (data[18] & 0b11111011) | (static_cast<byte>(value?1:0) << 2); return *this; }
                Discover& warmWhite(bool value) {data[18] = (data[18] & 0b11111101) | (static_cast<byte>(value?1:0) << 1); return *this; }
                Discover& coldWhite(bool value) {data[18] = (data[18] & 0b11111110) | static_cast<byte>(value?1:0); return *this; }
                Discover& MTU(uint16_t value) { *reinterpret_cast<uint16_t*>(data + 19) = value; return *this; }
                Discover& ledsPerMeter(byte value) { data[20] = value; return *this; }
                Discover& name(const DiscoverName& value) { return name((char*)value.name); }
                Discover& name(const char* value){
                    int i = 0;
                    for(; i < 40 && value[i] != '\0'; ++i) data[21 + i] = value[i];
                    for(;i<40;++i) data[21 + i] = 0; return *this;}

                uint64_t size() const { return 60; }
            private:
                byte* const data;
        };
    }
}