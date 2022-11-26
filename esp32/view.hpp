#pragma once

template <typename T>
class view {
public:
    view() : data(nullptr), elements(0) {}

    view(T* data, uint64_t elements)
        : data(data), elements(elements) {}

    uint64_t size(){ return this->elements; }
    T* get(){ return this->data; }

    T& operator[](uint64_t i) { return this->data[i];}

    operator bool() { return elements != 0;};

private:
    T* data;
    uint64_t elements;
};