#include "WifiAdapter_ESP.h"

// Constructor is explicitly defaulted in the header file, no need for implementation here.

bool WifiAdapterESP::init() {
    // Initialize the WiFi manager
    return wifiManager->autoConnect("WaterTorrentManager", "1234567890");
}

std::string WifiAdapterESP::getMacAddress() {
    // Get the MAC address of the ESP32 device
    uint8_t mac[6];
    WiFi.macAddress(mac);
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    isConnectedFlag = (WiFi.status() == WL_CONNECTED);
    return std::string(macStr);
}

std::vector<std::string> WifiAdapterESP::getAvailableNetworks() const {
    // Scan for available WiFi networks
    int n = WiFi.scanNetworks();
    std::vector<std::string> networks;
    for (int i = 0; i < n; ++i) {
        networks.push_back(WiFi.SSID(i).c_str());
    }
    return networks;
}

bool WifiAdapterESP::isConnected() const {
    // Check if the device is connected to a WiFi network
    return isConnectedFlag;
}