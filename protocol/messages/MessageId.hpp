#pragma once

namespace YALC{
    enum class MessageId {
        DISCOVER = 0x01,
        RESPONSE = 0x02,
        PING = 0x03,
        SET_COLORS_8BIT = 0xC8,
    };
}