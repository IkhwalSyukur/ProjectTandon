#pragma once
#include <string>

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
#include <interfaces/WaterServoInterface.h>
#include <interfaces/WaterPumpInterface.h>
#include <interfaces/WaterTimeInterface.h>
#include <interfaces/WaterFuzzyInterface.h>
#include <interfaces/WaterLevelSensorInterface.h>
#include <interfaces/WaterFlowSensorInterface.h>

#include "WaterTorrentPubSource.h"

class WaterTorrentManager
{
    public:
        // static WaterTorrentManager* instance;
        WaterTorrentManager(std::unique_ptr<WifiAdapterInterface> &&wifiAdapter, WaterLevelSensorInterface &waterLevel, WaterFlowSensorInterface &waterFlow, WaterPumpInterface &waterPump, WaterFuzzyInterface &waterFuzzy, WaterServoInterface &waterServo , WaterTimeInterface &waterTime)
            :_wifiAdapter(std::move(wifiAdapter)), _waterLevel(waterLevel), _waterFlow(waterFlow), _waterPump(waterPump), _waterFuzzy(waterFuzzy), _waterServo(waterServo), _waterTime(waterTime) {};
                
        //Water Level
        bool begin();
        void mqttLoop() { mqttHandler.loop(); }
        float getWaterLevel(); 
        std::string RelayDriver();

        //Water Servo
        void setAngle(int angle);

        //water flow
        void readingFlowRate();
        float getFlowRate();
        int getLitersPerDay();

        // water time
        int getHour();
        int getDay();

        //Fuzzy
        int runFuzzy();

    private:
        std::unique_ptr<WifiAdapterInterface> _wifiAdapter;
        MQTTHandler mqttHandler{MQTT_ID, MQTT_PUBLISH_TOPIC, MQTT_SUBSCRIBE_TOPIC};

        static void prepareSensorData(JsonDocument& doc);
        static WaterTorrentManager* instance;
        WaterLevelSensorInterface &_waterLevel;
        float waterLevel;

        WaterServoInterface &_waterServo;
        int angle;

        WaterFlowSensorInterface &_waterFlow;
        float flowRate;
        int litersPerDay;

        WaterTorrentData_t userData;

        WaterPumpInterface &_waterPump;
        bool pumpStatus = true;

        WaterTimeInterface &_waterTime; 
        int hour;
        int day;

        WaterFuzzyInterface &_waterFuzzy; 
        float volused = 0.0f; // Volume used in liters
};