#pragma once

#include "Common.hpp"
#include "MessageId.hpp"
#include <arpa/inet.h>

/*
 node_id 1 byte unsigned
 message_id 1 byte YALC::MessageId
 sequence_id 2 bytes unsigned

 total size: 4 bytes
*/

namespace YALC{
    uint32_t from(uint32_t value) {
        return ntohl(value);
    }

    uint32_t to(uint32_t value) {
        return htonl(value);
    }

    namespace reader {
        class Header {
            public:
                Header(byte* const ptr) : data(ptr){}
                byte nodeId() const { return data[0]; }
                MessageId messageId() const { return static_cast<MessageId>(data[1]);}
                uint16_t sequenceId() const { return *reinterpret_cast<uint16_t*>(data + 2);}

            private:
                byte* const data;
        };
    }

    namespace writer {
        class Header {
            public:
                Header(byte* ptr) : data(ptr){}
                Header& nodeId(byte value) { data[0] = value; return *this; }
                Header& messageId(MessageId value) { data[1] = static_cast<byte>(value); return *this;}
                Header& sequenceId(uint16_t value) { *reinterpret_cast<uint16_t*>(data+2) = value; return *this; }

                uint64_t size() { return 4; }

            private:
                byte* data;
        };
    }
}