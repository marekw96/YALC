#pragma once

#include "Common.hpp"
#include "Header.hpp"
#include "MessageId.hpp"

/*
 header Header 4 bytes YALC::Header
 sequance 4 bytes unsigned
*/

namespace YALC {
    namespace reader {
        class Ping {
            public:
                Ping(const byte* ptr) : data(const_cast<byte*>(ptr)) {}
                Header header() const { return Header(data);}
                uint32_t sequence() const { return from(*reinterpret_cast<uint32_t*>(data+4)); }
            private:
                byte* data;
        };
    }

    namespace writer {
        class Ping {
            public:
                Ping(byte* const ptr) : data(ptr) {}
                Header header() { return Header(data); }
                Ping& sequence(uint32_t value) { *reinterpret_cast<uint32_t*>(data + 4) = to(value); return *this; }
                uint64_t size() const { return 8; }
            private:
                byte* const data;
        };
    }
}