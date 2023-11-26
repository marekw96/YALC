#include "Storage.hpp"

#include "pico_hal.h"
#include "pico/multicore.h"

constexpr auto ONE_SEC_IN_US = 1'000'000;

void dump_stats(const char* fnc) {
    struct pico_fsstat_t stat;
    pico_fsstat(&stat);
    printf("FS[%s]: blocks %d, block size %d, used %d\n", fnc, (int)stat.block_count, (int)stat.block_size,
           (int)stat.blocks_used);
}

bool Storage::init(FormatAction format)
{
    bool shouldFormat = format == FormatAction::FORMAT;

    return pico_mount(shouldFormat) == LFS_ERR_OK;
}

bool Storage::store(const char *path, uint32_t value)
{
    //remount();
    dump_stats("store::uint32_t");

    int file = pico_open(path, LFS_O_RDWR | LFS_O_CREAT);
    if(file < 0) {
        return false;
    }
    if(!multicore_lockout_start_timeout_us(ONE_SEC_IN_US)) {
        printf("Failed to enter to lockout state!\n");
    }

    auto bytesWritten = pico_write(file, &value, sizeof(value));
    if(bytesWritten < 0) {
        printf("Storage::store(uint32_t)[%s] write failed: %s\n", path, pico_errmsg(bytesWritten));
    }
    auto result = pico_fflush(file);
    if(result < 0 ) {
        printf("Storage::store(uint32_t)[%s] flush failed: %s\n", path, pico_errmsg(result));
    }
    multicore_lockout_end_blocking();
    result = pico_close(file);
    if(result < 0 ) {
        printf("Storage::store(uint32_t)[%s] close failed: %s\n", path, pico_errmsg(result));
    }


    return bytesWritten == sizeof(value);
}

bool Storage::store(const char *path, const std::string &value)
{
    //remount();
    dump_stats("store::string");
    printf("Store[String] at %s with size %d\n", path, value.size());

    int file = pico_open(path, LFS_O_RDWR | LFS_O_CREAT);
    if(file < 0) {
        return false;
    }

    if(!multicore_lockout_start_timeout_us(ONE_SEC_IN_US)) {
        printf("Failed to enter to lockout state!\n");
    }
    auto bytesWritten = pico_write(file, value.c_str(), value.size());
    if(bytesWritten < 0) {
        printf("Storage::store(string)[%s] write failed: %s\n", path, pico_errmsg(bytesWritten));
    }
    auto result = pico_fflush(file);
    if(result < 0 ) {
        printf("Storage::store(string)[%s] flush failed: %s\n", path, pico_errmsg(result));
    }
    multicore_lockout_end_blocking();
    result = pico_close(file);
    if(result < 0 ) {
        printf("Storage::store(string)[%s] close failed: %s\n", path, pico_errmsg(result));
    }


    return bytesWritten == value.size();
}

uint32_t Storage::read_uint32_t(const char *path)
{
    printf("Storage::read<uint32> %s\n", path);
    uint32_t value = 0;
    int file = pico_open(path, LFS_O_RDWR | LFS_O_CREAT);
    if(file < 0) { return 0; }
    pico_read(file, &value, sizeof(value));
    pico_close(file);
    return value;
}

std::string Storage::read_string(const char *path)
{
    int file = pico_open(path, LFS_O_RDWR | LFS_O_CREAT);
    if(file < 0) {
        printf("Storage::read<string>[%s] failed to open %s\n", path, pico_errmsg(file));
        return "";
    }
    auto size = pico_size(file);
    std::string value(size+1, 0);
    pico_read(file, &value[0], size);
    pico_close(file);

    return value;
}

bool Storage::makeDir(const char *path, bool block)
{
    dump_stats("store::makeDir");
    printf("Store::makeDir %s %s\n", path, block?"Blocking":"Non-Blocking");

    if(block){
        if(!multicore_lockout_start_timeout_us(ONE_SEC_IN_US)) {
            printf("Failed to enter to lockout state!\n");
        }
    }
    auto result = pico_mkdir(path);
    if(block)
        multicore_lockout_end_blocking();

    if(result < 0) {
        printf("Storage::makeDir[%s] mkdir failed: %s\n", path, pico_errmsg(result));
    }

    return result == LFS_ERR_OK;
}

void Storage::remount()
{
    auto result = pico_unmount();
    if(result < 0 ) {
        printf("Storage::remount unmount failed: %s\n", pico_errmsg(result));
    }

    result = pico_mount(false);
    if(result < 0 ) {
        printf("Storage::remount mount failed: %s\n", pico_errmsg(result));
    }
}
