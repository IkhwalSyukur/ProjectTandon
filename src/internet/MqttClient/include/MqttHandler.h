#pragma once

#include "MqttHandlerDefinition.h"
#include <mqtt_client.h>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include "esp_log.h"
#include "esp_err.h"
#include "esp_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"

using MqttClientConnectedCallback = std::function<void()>;
using MqttClientDisconnectedCallback = std::function<void()>;

class MqttHandler
{
    public:
        bool init(const std::string& broker, const std::string& clientId, const std::string& username, const std::string& password, int port);
        bool reconnect();
        void disconnect();
        bool isConnected() const;
        bool subscribe(const std::string& topic, int qos = CONFIG_MQTT_QOS_DEFAULT);
        bool unsubscribe(const std::string& topic);
        bool publish(const std::string& topic, const std::string& message, int qos = CONFIG_MQTT_QOS_DEFAULT, bool retain = CONFIG_MQTT_RETAIN_MESSAGES);
        bool enqueueMessage(const std::string& topic, const std::string& message, int qos = CONFIG_MQTT_QOS_DEFAULT, bool retain = CONFIG_MQTT_RETAIN_MESSAGES);

        void setOnConnectedCallback(MqttClientConnectedCallback callback) { onConnectedCallback = callback; }
        void setOnDisconnectedCallback(MqttClientDisconnectedCallback callback) { onDisconnectedCallback = callback; }
        
    private:
        void onMqttConnected();
        void onMqttDisconnected();
        static void mqttEventHandler(void* handler_args, esp_event_base_t base, int32_t id, void* event_data);

        esp_mqtt_client_handle_t client;
        esp_mqtt_client_config_t mqtt_cfg;
        bool connected = false;
        EventGroupHandle_t mqttEventGroup = nullptr;

        MqttClientConnectedCallback onConnectedCallback = nullptr;
        MqttClientDisconnectedCallback onDisconnectedCallback = nullptr;
};
