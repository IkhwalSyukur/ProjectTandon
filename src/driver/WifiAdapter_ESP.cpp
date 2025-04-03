#include "WifiAdapter_ESP.h"

// Constructor is explicitly defaulted in the header file, no need for implementation here.

bool WifiAdapterESP::init() {
    // Initialize the WiFi manager
    return wifiManager->autoConnect("WaterTorrentManager", "1234567890");
}