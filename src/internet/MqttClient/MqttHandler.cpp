#include "include/MqttHandler.h"

bool MqttHandler::init(const std::string& broker, const std::string& clientId, const std::string& username, const std::string& password, int port)
{
    mqtt_cfg.host = broker.c_str();
    mqtt_cfg.client_id = clientId.c_str();
    mqtt_cfg.username = username.c_str();
    mqtt_cfg.password = password.c_str();
    mqtt_cfg.port = port;
    mqtt_cfg.keepalive = 60;
    mqtt_cfg.buffer_size = 1024 * 4; // 4 KB buffer size
    mqtt_cfg.out_buffer_size = 1024 * 4; // 4 KB output buffer size
    mqtt_cfg.task_stack = 4096; // 4 KB stack size for the task

    mqttEventGroup = xEventGroupCreate();
    if (mqttEventGroup == nullptr)
    {
        ESP_LOGE(CONFIG_MQTT_HANDLER_TAG, "Failed to create event group for MQTT handler.");
        return false;
    }

    client = esp_mqtt_client_init(&mqtt_cfg);
    if (client == nullptr)
    {
        ESP_LOGE(CONFIG_MQTT_HANDLER_TAG, "Failed to initialize MQTT client.");
        return false;
    }

    esp_mqtt_client_register_event(client, (esp_mqtt_event_id_t)ESP_EVENT_ANY_ID, mqttEventHandler, this);
    esp_err_t err = esp_mqtt_client_start(client);
    if (err != ESP_OK)
    {
        ESP_LOGE(CONFIG_MQTT_HANDLER_TAG, "Failed to start MQTT client: %s", esp_err_to_name(err));
        return false;
    }

    ESP_LOGI(CONFIG_MQTT_HANDLER_TAG, "MQTT client initialized successfully.");
    xEventGroupClearBits(mqttEventGroup, CONFIG_MQTT_EVENT_CONNECTED_BIT | CONFIG_MQTT_EVENT_DISCONNECTED_BIT | CONFIG_MQTT_EVENT_PUBLISHED_BIT);
    xEventGroupClearBits(mqttEventGroup, CONFIG_MQTT_EVENT_SUBSCRIBED_BIT | CONFIG_MQTT_EVENT_UNSUBSCRIBED_BIT | CONFIG_MQTT_EVENT_DATA_BIT);
    xEventGroupClearBits(mqttEventGroup, CONFIG_MQTT_EVENT_ERROR_BIT | CONFIG_MQTT_EVENT_BEFORE_CONNECT_BIT | CONFIG_MQTT_EVENT_DELETED);

    return true;
}

bool MqttHandler::reconnect()
{
    if (connected)
    {
        ESP_LOGI(CONFIG_MQTT_HANDLER_TAG, "Already connected to MQTT broker.");
        return true;
    }

    ESP_ERROR_CHECK(esp_mqtt_client_reconnect(client));
    EventBits_t bits = xEventGroupWaitBits(mqttEventGroup, CONFIG_MQTT_EVENT_CONNECTED_BIT | CONFIG_MQTT_EVENT_DISCONNECTED_BIT, pdTRUE, pdFALSE, portMAX_DELAY);
    ESP_LOGI(CONFIG_MQTT_HANDLER_TAG, "Reconnecting to MQTT broker...");
    return true;
}

void MqttHandler::disconnect()
{
    if (client != nullptr)
    {
        esp_mqtt_client_stop(client);
        esp_mqtt_client_destroy(client);
        client = nullptr;
    }
    connected = false;
    ESP_LOGI(CONFIG_MQTT_HANDLER_TAG, "Disconnected from MQTT broker.");
}

bool MqttHandler::isConnected() const
{
    return connected;
}

bool MqttHandler::subscribe(const std::string& topic, int qos)
{
    if (client == nullptr)
    {
        ESP_LOGE(CONFIG_MQTT_HANDLER_TAG, "MQTT client is not initialized.");
        return false;
    }

    int msg_id = esp_mqtt_client_subscribe(client, topic.c_str(), qos);
    if (msg_id < 0)
    {
        ESP_LOGE(CONFIG_MQTT_HANDLER_TAG, "Failed to subscribe to topic %s: %s", topic.c_str(), esp_err_to_name(msg_id));
        return false;
    }

    ESP_LOGI(CONFIG_MQTT_HANDLER_TAG, "Subscribed to topic %s with msg_id %d", topic.c_str(), msg_id);
    return true;
}

bool MqttHandler::unsubscribe(const std::string& topic)
{
    if (client == nullptr)
    {
        ESP_LOGE(CONFIG_MQTT_HANDLER_TAG, "MQTT client is not initialized.");
        return false;
    }

    int msg_id = esp_mqtt_client_unsubscribe(client, topic.c_str());
    if (msg_id < 0)
    {
        ESP_LOGE(CONFIG_MQTT_HANDLER_TAG, "Failed to unsubscribe from topic %s: %s", topic.c_str(), esp_err_to_name(msg_id));
        return false;
    }

    ESP_LOGI(CONFIG_MQTT_HANDLER_TAG, "Unsubscribed from topic %s with msg_id %d", topic.c_str(), msg_id);
    return true;
}

bool MqttHandler::publish(const std::string& topic, const std::string& message, int qos, bool retain)
{
    if (client == nullptr)
    {
        ESP_LOGE(CONFIG_MQTT_HANDLER_TAG, "MQTT client is not initialized.");
        return false;
    }

    int msg_id = esp_mqtt_client_publish(client, topic.c_str(), message.c_str(), message.length(), qos, retain);
    if (msg_id < 0)
    {
        ESP_LOGE(CONFIG_MQTT_HANDLER_TAG, "Failed to publish message to topic %s: %s", topic.c_str(), esp_err_to_name(msg_id));
        return false;
    }

    ESP_LOGI(CONFIG_MQTT_HANDLER_TAG, "Published message to topic %s with msg_id %d", topic.c_str(), msg_id);
    return true;
}

bool MqttHandler::enqueueMessage(const std::string& topic, const std::string& message, int qos, bool retain)
{
    if (client == nullptr)
    {
        ESP_LOGE(CONFIG_MQTT_HANDLER_TAG, "MQTT client is not initialized.");
        return false;
    }

    int msg_id = esp_mqtt_client_enqueue(client, topic.c_str(), message.c_str(), message.length(), qos, retain, CONFIG_MQTT_QOS0_STORE);
    if (msg_id < 0)
    {
        ESP_LOGE(CONFIG_MQTT_HANDLER_TAG, "Failed to enqueue message to topic %s: %s", topic.c_str(), esp_err_to_name(msg_id));
        return false;
    }

    ESP_LOGI(CONFIG_MQTT_HANDLER_TAG, "Enqueued message to topic %s with msg_id %d", topic.c_str(), msg_id);
    return true;
}

void MqttHandler::onMqttConnected()
{
    connected = true;
    ESP_LOGI(CONFIG_MQTT_HANDLER_TAG, "MQTT client connected to broker.");
    if (onConnectedCallback)
    {
        onConnectedCallback();
    }
}

void MqttHandler::onMqttDisconnected()
{
    connected = false;
    ESP_LOGI(CONFIG_MQTT_HANDLER_TAG, "MQTT client disconnected from broker.");
    if (onDisconnectedCallback)
    {
        onDisconnectedCallback();
    }
}

void MqttHandler::mqttEventHandler(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{
    MqttHandler* mqttHandler = static_cast<MqttHandler*>(handler_args);
    esp_mqtt_event_handle_t event = static_cast<esp_mqtt_event_handle_t>(event_data);

    switch (id)
    {
        case MQTT_EVENT_CONNECTED:
            mqttHandler->onMqttConnected();
            break;
        case MQTT_EVENT_DISCONNECTED:
            mqttHandler->onMqttDisconnected();
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(CONFIG_MQTT_HANDLER_TAG, "Subscribed to topic %s", event->topic);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(CONFIG_MQTT_HANDLER_TAG, "Unsubscribed from topic %s", event->topic);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(CONFIG_MQTT_HANDLER_TAG, "Message published to topic %s", event->topic);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(CONFIG_MQTT_HANDLER_TAG, "Received data on topic %s: %.*s", event->topic, event->data_len, event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(CONFIG_MQTT_HANDLER_TAG, "MQTT_EVENT_ERROR");
            break;
        case MQTT_EVENT_BEFORE_CONNECT:
            ESP_LOGI(CONFIG_MQTT_HANDLER_TAG, "MQTT_EVENT_BEFORE_CONNECT");
            break;
        default:
            ESP_LOGE(CONFIG_MQTT_HANDLER_TAG, "Unhandled MQTT event: %d", id);
            break;
    }
}

