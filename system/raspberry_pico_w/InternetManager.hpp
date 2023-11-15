#pragma once

class InternetManager {
public:
    void init();
    bool connect_to(const char* ssid, const char* passwd);
    void deinit();
};