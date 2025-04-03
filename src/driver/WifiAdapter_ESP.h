#pragma once

#include <interfaces/WifiAdapterInterface.h>
#include <string>
#include <vector>
#include <memory>
#include <WiFiManager.h>

class WifiAdapterESP : public WifiAdapterInterface
{ 
    public:
        WifiAdapterESP() = default;
        ~WifiAdapterESP() override = default;

        bool init() override;
        // std::string getName() const override;
        std::string getMacAddress() override;
        std::vector<std::string> getAvailableNetworks() const override;
        // bool connectToNetwork(const std::string& ssid, const std::string& password) override;
        // bool disconnectFromNetwork() override;
        bool isConnected() const override;
        // std::string getCurrentNetwork() const override;

    private:
        std::unique_ptr<WiFiManager> wifiManager = std::make_unique<WiFiManager>();
        std::string currentNetwork;
        std::string currentMacAddress;
        bool isConnectedFlag = false;
};