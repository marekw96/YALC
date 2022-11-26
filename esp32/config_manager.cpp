#include "config_manager.hpp"

#include "SD_MMC.h"
#include <stdio.h>

config_manager::config_manager()
    : fs(SD_MMC), ready(false)
{}

config_manager::~config_manager()
{
    this->ready = false;
    Serial.println("config_manager::~config_manager");
    SD_MMC.end();
}

bool config_manager::begin() {
    if(!SD_MMC.begin())
        return false;

    this->ready = true;
    return true;
}

bool config_manager::store_value(const char* object_name, const char* name, const char* value) {
    if(!this->is_ok())
        return false;

    const int path_size = 100;
    char directory_path[path_size];
    char file_path[path_size*2];

    if(snprintf(directory_path, path_size, "/%s", object_name) <= 0) {
        return false;
    }

    if(snprintf(file_path, path_size*2, "/%s/%s.prop", object_name, name) <= 0) {
        return false;
    }

    if(!this->fs.exists(directory_path))
    {
        if(!this->fs.mkdir(directory_path))
        {
            Serial.printf("mkdir failed: %s\n\r", directory_path);
        }
    }

    auto file = this->fs.open(file_path, "w");
    if(file)
    {
        file.write(reinterpret_cast<const uint8_t*>(value), strlen(value));
        file.close();
        return true;
    }

    return false;
}

String config_manager::read_value(const char* object_name, const char* name) {
    if(!this->is_ok())
        return {};

    const int path_size = 100;
    char file_path[path_size*2];

    if(snprintf(file_path, path_size*2, "/%s/%s.prop", object_name, name) <= 0) {
        return {};
    }

    auto file = this->fs.open(file_path);
    if(!file)
    {
        return {};
    }

    uint32_t length = file.size();
    String txt;
    char buffer[100];
    while(length) {
        uint32_t to_read = length;
        if(to_read > 100)
            to_read = 100;

        file.read(reinterpret_cast<uint8_t*>(buffer), to_read);
        //WFT!!!!!!?
        for(int i = 0; i < to_read; ++i)
            txt += buffer[i];

        length -= to_read;
    }

    file.close();
    return txt;
}

bool config_manager::is_ok() {
    return this->ready;
}