#include <Arduino.h>
#include "WaterTorrentManager.h"
#include "esp_log.h"

// WaterTorrentManager.cpp
#include "WaterTorrentManager.h"

const char* MQTT_SERVER = "test.mosquitto.org"; // Public MQTT broker
const int MQTT_PORT = 1883;
const char* MQTT_ID = "WaterTorrentManager";
const char* MQTT_PUBLISH_TOPIC = "water_torrent_armisuari/sensorData";
const char* MQTT_SUBSCRIBE_TOPIC = "water_torrent_armisuari/control";

static const char *TAG = "WaterTorrentManager";
WaterTorrentManager* WaterTorrentManager::instance = nullptr;

WaterTorrentManager::WaterTorrentManager(std::unique_ptr<WifiAdapterInterface> wifiAdapter)
    : _wifiAdapter(std::move(wifiAdapter))
{
    if (_wifiAdapter == nullptr)
    {
        ESP_LOGE(TAG, "Failed to initialize WaterTorrentManager: Null pointer provided.");
    }

    instance = this;
}

bool WaterTorrentManager::begin()
{
    if (!_wifiAdapter->init())
    {
        ESP_LOGE(TAG, "Failed to initialize WiFi Adapter.");
        return false;
    }

    ESP_LOGI(TAG, "MAC Address: %s", _wifiAdapter->getMacAddress().c_str());
    
    ESP_LOGI(TAG, "Available Networks:");
    auto networks = _wifiAdapter->getAvailableNetworks();
    for (const auto &network : networks)
    {
        ESP_LOGI(TAG, " - %s", network.c_str());
    }

    vTaskDelay(2000); // Allow time for WiFi to stabilize
    if (!_wifiAdapter->isConnected())
    {
        ESP_LOGE(TAG, "WiFi is not connected. Please check your network settings.");
        return false;
    }

    // setup time from NTP
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    mqttHandler.begin(
        MQTT_SERVER,
        MQTT_PORT
    );

    // Start periodic sending with our data preparation callback
    mqttHandler.startSendTask(prepareSensorData);

    // Send initial status
    mqttHandler.sendString("{\"status\":\"online\"}");

    return true;
}

void WaterTorrentManager::prepareSensorData(JsonDocument& doc) {
    // Simulate sensor readings
    float temperature = 25.0 + (random(0, 10) / 10.0);
    float humidity = 40.0 + (random(0, 20) / 10.0);

    doc["device_id"] = MQTT_ID + std::string("_") + instance->_wifiAdapter->getMacAddress();
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    doc["timestamp"] = time(nullptr); // Current time in seconds since epoch
}