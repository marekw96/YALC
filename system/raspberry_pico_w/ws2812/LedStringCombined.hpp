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

    YALC::ColorOrder colorOrder(){
        return a.colorOrder(); //For now lets assume that both have the same
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