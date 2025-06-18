#pragma once
#include <string>

#include <interfaces/WaterLevelSensorInterface.h>
#include <interfaces/WaterFlowSensorInterface.h>
#include <interfaces/WaterServoInterface.h>
#include <interfaces/WaterPumpInterface.h>
#include <interfaces/WaterTimeInterface.h>
#include <interfaces/WaterFuzzyInterface.h>

#include "WaterTorrentPubSource.h"

class WaterTorrentManager
{
    public:
        WaterTorrentManager(WaterLevelSensorInterface &waterLevel, WaterFlowSensorInterface &waterFlow, WaterPumpInterface &waterPump, WaterFuzzyInterface &waterFuzzy, WaterServoInterface &waterServo , WaterTimeInterface &waterTime)
            : _waterLevel(waterLevel), _waterFlow(waterFlow), _waterPump(waterPump), _waterFuzzy(waterFuzzy), _waterServo(waterServo), _waterTime (waterTime) {};
                
        //Water Level
        bool begin();
        void readingWaterLevel();
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