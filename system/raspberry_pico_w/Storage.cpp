#include "Storage.hpp"

#include "pico_hal.h"

bool Storage::init(FormatAction format)
{
    bool shouldFormat = format == FormatAction::FORMAT;

    return pico_mount(shouldFormat) == LFS_ERR_OK;
}

bool Storage::store(const char *path, uint32_t value)
{
    int file = pico_open(path, LFS_O_RDWR | LFS_O_CREAT);
    if(file < 0) {
        return false;
    }
    auto bytesWritten = pico_write(file, &value, sizeof(value));
    pico_close(file);

    printf("Store %s %d %d\n", path, value, bytesWritten);

    return bytesWritten == sizeof(value);
}

uint32_t Storage::read_uint32_t(const char *path)
{
    uint32_t value;
    int file = pico_open(path, LFS_O_RDWR | LFS_O_CREAT);
    if(file < 0) { return 0; }
    pico_read(file, &value, sizeof(value));
    pico_close(file);
    return value;
}

bool Storage::makeDir(const char *path)
{
    return pico_mkdir(path) == LFS_ERR_OK;
}
