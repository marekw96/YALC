#pragma once

template <typename A, typename B>
class LedStringCombined {
public:
    LedStringCombined(A& a, B& b) : a(a), b(b){}
    void init() {
        a.init();
        b.init();
    }

    void update() {
        a.update();
        b.update();
    }

    uint64_t ledsCount() const {
        return a.ledsCount() + b.ledsCount();
    }

    void setPixel(uint32_t pixel, uint8_t red, uint8_t green, uint8_t blue) {
        //python animations for now only sets one pixel at time.
        //In future it needs to be done better!

        if(pixel < a.ledsCount()) {
            a.setPixel(pixel, red, green, blue);
        }
        else if(pixel < a.ledsCount() + b.ledsCount()) {
            b.setPixel(pixel, red, green, blue);
        }
    }

    void setColors(uint32_t startPixel, byte* data, uint32_t numberOfPixels) {
        //python animations for now only sets one pixel at time.
        //In future it needs to be done better!

        if(startPixel < a.ledsCount()) {
            a.setColors(startPixel, data, numberOfPixels);
        }
        else if(startPixel < a.ledsCount() + b.ledsCount()) {
            b.setColors(startPixel - a.ledsCount(), data, numberOfPixels);
        }
        //drop otherwise
    }

private:
    A& a;
    B& b;
};