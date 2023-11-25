#pragma once
#define LIB_PICO_MULTICORE 1
#include <stdio.h>
#include "pico/stdlib.h"
#include <string>

class Storage {
public:
    enum FormatAction {FORMAT, DO_NOT_FORMAT};
    bool init(FormatAction format);

    bool store(const char* path, uint32_t value);
    bool store(const char* path, const std::string& value);
    uint32_t read_uint32_t(const char* path);
    std::string read_string(const char* path);
    bool makeDir(const char* path, bool block = true);

private:
    void remount();
};