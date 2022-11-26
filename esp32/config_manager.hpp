#pragma once

#include "FS.h"
#include <WString.h>

class config_manager {
public:
    config_manager();
    ~config_manager();
    bool begin();
    bool store_value(const char* object_name, const char* name, const char* value);
    String read_value(const char* object_name, const char* name);
    bool is_ok();
    operator bool(){ return is_ok(); }

private:
    fs::FS &fs;
    bool ready;
};