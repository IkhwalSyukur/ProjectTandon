#pragma once

#include <string>
#include <vector>
#include <memory>

class WifiAdapterInterface 
{
    public:
        virtual ~WifiAdapterInterface() = default;
        virtual bool init() = 0;
        virtual std::string getMacAddress() = 0;
        virtual std::vector<std::string> getAvailableNetworks() const = 0;
        virtual bool isConnected() const = 0;
        virtual bool disconnectFromNetwork() = 0;
        // virtual std::string getName() const = 0;
        // virtual bool connectToNetwork(const std::string& ssid, const std::string& password) = 0;
        // virtual std::string getCurrentNetwork() const = 0;
};