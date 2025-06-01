#include "MQTTHandler.h"

// MQTT callback for incoming messages
void MQTTHandler::mqttCallback(char* topic, byte* payload, unsigned int length) {
    char message[length + 1];
    memcpy(message, payload, length);
    message[length] = '\0';
    
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.println(message);
}

void MQTTHandler::reconnect() {
    while (!mqttClient.connected()) {
        Serial.print("Attempting MQTT connection...");
        
        if (mqttClient.connect(clientId)) {
            Serial.println("connected");
            
            if (subscribeTopic != nullptr) {
                mqttClient.subscribe(subscribeTopic);
                Serial.print("Subscribed to: ");
                Serial.println(subscribeTopic);
            }
        } else {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

MQTTHandler::MQTTHandler(const char* id, const char* pubTopic, const char* subTopic) 
    : clientId(id), publishTopic(pubTopic), subscribeTopic(subTopic) {
    mqttClient.setClient(wifiClient);
}

void MQTTHandler::begin( const char* mqtt_server, int mqtt_port, const char* mqtt_user, const char* mqtt_pass) {
    mqttClient.setServer(mqtt_server, mqtt_port);
    mqttClient.setCallback(mqttCallback);
    
    // WiFi.begin(wifi_ssid, wifi_pass);
    // while (WiFi.status() != WL_CONNECTED) {
    //     delay(1000);
    //     Serial.println("Connecting to WiFi...");
    // }
    // Serial.println("WiFi connected");
    // Serial.print("IP address: ");
    // Serial.println(WiFi.localIP());

    reconnect();
}

void MQTTHandler::sendTaskFunction(void* param) {
    auto handler = static_cast<MQTTHandler*>(param);
    
    while (handler->taskRunning) {
        if (handler->isConnected()) {
            StaticJsonDocument<256> doc;
            handler->sendCallback(doc);
            handler->sendJson(doc);
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
    vTaskDelete(NULL);
}

void MQTTHandler::startSendTask(void (*callback)(JsonDocument&)) {
    if (sendTaskHandle == NULL) {
        sendCallback = callback;
        taskRunning = true;
        xTaskCreate(
            sendTaskFunction,
            "MQTTSendTask",
            4096,
            this,
            1,
            &sendTaskHandle
        );
    }
}

void MQTTHandler::stopSendTask() {
    taskRunning = false;
    if (sendTaskHandle != NULL) {
        vTaskDelete(sendTaskHandle);
        sendTaskHandle = NULL;
    }
}

void MQTTHandler::sendJson(const JsonDocument& doc) {
    char jsonBuffer[256];
    serializeJson(doc, jsonBuffer);
    
    if (isConnected()) {
        mqttClient.publish(publishTopic, jsonBuffer);
        Serial.print("Published: ");
        Serial.println(jsonBuffer);
    }
}

void MQTTHandler::sendString(const char* message) {
    if (isConnected()) {
        mqttClient.publish(publishTopic, message);
        Serial.print("Published: ");
        Serial.println(message);
    }
}

void MQTTHandler::loop() {
    if (!isConnected()) {
        reconnect();
    }
    mqttClient.loop();
}

bool MQTTHandler::isConnected() {
    return mqttClient.connected();
}

MQTTHandler::~MQTTHandler() {
    stopSendTask();
    if (isConnected()) {
        mqttClient.disconnect();
    }
}