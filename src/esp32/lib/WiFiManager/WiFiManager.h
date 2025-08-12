#pragma once

#include <WiFi.h>
#include "ConfigLoader.h"

class WiFiManager {
public:
    static void begin();
    static bool isConnected();
    static void reconnectIfNeeded();

private:
    static bool connected;
};