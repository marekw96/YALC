#pragma once

#include "Common.hpp"
#include "Header.hpp"

/*
 header Header 4 bytes YALC::Header
 start_pixel 4 bytes unsigned
 number_of_pixels 4 bytes unsigned
 pixel_data array of PixelData8Bit, length defined by number_of_pixels
*/

namespace YALC {
    namespace reader {
        class SetColors8Bit {
            public:
                SetColors8Bit(const byte* ptr) : data(const_cast<byte*>(ptr)) {}
                Header header() const { return Header(data);}
                uint32_t startPixel() const { return *reinterpret_cast<uint32_t*>(data+4); }
                uint32_t numberOfPixels() const { return from(*reinterpret_cast<uint32_t*>(data+8)); }
                const PixelData8Bit& getPixel(int pixel) const { return *reinterpret_cast<PixelData8Bit*>(data + 12 + pixel*3);}
                PixelData8Bit* getPixelsArray() const { return reinterpret_cast<PixelData8Bit*>(data+12);}
            private:
                byte* data;
        };
    }

    namespace writer {
        class SetColors8Bit {
            public:
                SetColors8Bit(byte* const ptr) : data(ptr) {}
                Header header() { return Header(data); }
                SetColors8Bit& startPixel(uint32_t value) { *reinterpret_cast<uint32_t*>(data + 4) = value; return *this; }
                SetColors8Bit& numberOfPixels(uint32_t value) { *reinterpret_cast<uint32_t*>(data + 8) = to(value); return *this; }
                const PixelData8Bit& getPixel(int pixel) const { return *reinterpret_cast<PixelData8Bit*>(data + 12 + pixel*3);}
                PixelData8Bit* getPixelsArray() const { return reinterpret_cast<PixelData8Bit*>(data+12);}
            private:
                byte* const data;
        };
    }
}