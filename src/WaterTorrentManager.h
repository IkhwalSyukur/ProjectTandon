#pragma once

#include <interfaces/WifiAdapterInterface.h>
#include <memory>
#include <string>
#include "esp_log.h"

#include "internet/MqttClient/include/MqttHandler.h"

class WaterTorrentManager
{
    public:
        WaterTorrentManager(std::unique_ptr<WifiAdapterInterface> wifiAdapter);
        bool begin();

    private:
        std::unique_ptr<WifiAdapterInterface> _wifiAdapter;
        MqttHandler _mqttHandler;

        bool setupMqtt();
        void onMqttConnected();
        void onMqttDisconnected();
};