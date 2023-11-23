#pragma once

#include <stdio.h>
#include "pico/stdlib.h"

class Storage {
public:
    enum FormatAction {FORMAT, DO_NOT_FORMAT};
    bool init(FormatAction format);

    bool store(const char* path, uint32_t value);
    uint32_t read_uint32_t(const char* path);
    bool makeDir(const char* paht);
};