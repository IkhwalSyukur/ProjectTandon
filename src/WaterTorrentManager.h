#pragma once

#include <interfaces/WifiAdapterInterface.h>
#include <memory>
#include <string>
#include "esp_log.h"
#include <MQTTHandler.h>

// WaterTorrentManager.h
extern const char* MQTT_SERVER;
extern const int MQTT_PORT;
extern const char* MQTT_USER;
extern const char* MQTT_PASS;
extern const char* MQTT_ID;
extern const char* MQTT_PUBLISH_TOPIC;
extern const char* MQTT_SUBSCRIBE_TOPIC;

class WaterTorrentManager
{
    public:
        WaterTorrentManager(std::unique_ptr<WifiAdapterInterface> wifiAdapter);
        bool begin();
        void mqttLoop() { mqttHandler.loop(); }

    private:
        std::unique_ptr<WifiAdapterInterface> _wifiAdapter;
        MQTTHandler mqttHandler{MQTT_ID, MQTT_PUBLISH_TOPIC, MQTT_SUBSCRIBE_TOPIC};

        static void prepareSensorData(JsonDocument& doc);
        static WaterTorrentManager* instance;
};