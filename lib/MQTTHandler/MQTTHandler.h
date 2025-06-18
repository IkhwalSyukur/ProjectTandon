#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

class MQTTHandler {
private:
    WiFiClient wifiClient;
    PubSubClient mqttClient;
    TaskHandle_t sendTaskHandle = NULL;
    bool taskRunning = false;
    const char* clientId;
    const char* publishTopic;
    const char* subscribeTopic;

    static void mqttCallback(char* topic, byte* payload, unsigned int length);
    void reconnect();

public:
    void (*sendCallback)(JsonDocument&) = nullptr;
    static void sendTaskFunction(void* param);

    MQTTHandler(const char* id, const char* pubTopic, const char* subTopic = nullptr);
    
    void begin(const char* mqtt_server, int mqtt_port,
              const char* mqtt_user = nullptr, const char* mqtt_pass = nullptr);
    
    void startSendTask(void (*sendCallback)(JsonDocument&));
    void stopSendTask();
    
    void sendJson(const JsonDocument& doc);
    void sendString(const char* message);
    
    void loop();
    bool isConnected();
    
    ~MQTTHandler();
};

#endif // MQTT_HANDLER_H