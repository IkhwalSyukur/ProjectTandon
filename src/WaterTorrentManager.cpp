#include <Arduino.h>
#include "WaterTorrentManager.h"
#include "esp_log.h"

static const char *TAG = "WaterTorrentManager";

WaterTorrentManager::WaterTorrentManager(std::unique_ptr<WifiAdapterInterface> wifiAdapter)
    : _wifiAdapter(std::move(wifiAdapter))
{
    if (_wifiAdapter == nullptr)
    {
        ESP_LOGE(TAG, "Failed to initialize WaterTorrentManager: Null pointer provided.");
    }
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

    if (!setupMqtt())
    {
        ESP_LOGE(TAG, "Failed to set up MQTT handler.");
        return false;
    }

    return true;
}

bool WaterTorrentManager::setupMqtt()
{
    const std::string broker = CONFIG_MQTT_HANDLER_BROKER;
    const std::string clientId = CONFIG_MQTT_HANDLER_PREFIX_CLIENT_ID + _wifiAdapter->getMacAddress();
    const std::string username = CONFIG_MQTT_HANDLER_USERNAME;
    const std::string password = CONFIG_MQTT_HANDLER_PASSWORD;
    const int port = CONFIG_MQTT_HANDLER_PORT;

    ESP_LOGI(TAG, "Setting up MQTT with broker: %s, clientId: %s", broker.c_str(), clientId.c_str());

    if (!_mqttHandler.init(broker, clientId, username, password, port))
    {
        ESP_LOGE(TAG, "Failed to initialize MQTT handler. Broker: %s, Port: %d", broker.c_str(), port);
        return false;
    }

    _mqttHandler.setOnConnectedCallback([this]() { onMqttConnected(); });
    _mqttHandler.setOnDisconnectedCallback([this]() { onMqttDisconnected(); });

    ESP_LOGI(TAG, "MQTT handler initialized successfully.");
    _mqttHandler.publish("waterTorrent/sensor", "Device started", CONFIG_MQTT_QOS_DEFAULT, CONFIG_MQTT_RETAIN_MESSAGES);

    return true;
}

void WaterTorrentManager::onMqttConnected()
{
    // _mqttHandler.subscribe("waterTorrent/sensor", CONFIG_MQTT_QOS_DEFAULT);
}

void WaterTorrentManager::onMqttDisconnected()
{
    ESP_LOGI(TAG, "MQTT client disconnected. Attempting to reconnect...");
}