#include "WaterTorrentManager.h"
#include "esp_log.h"
#include <string>

static const char *TAG = "WaterTorrentManager";


// WaterTorrentManager::WaterTorrentManager(WaterLevelSensorInterface &waterLevel, WaterServoInterface &waterServo, WaterFlowSensorInterface &waterFlow, WaterPumpInterface &waterPump, WaterTimeInterface &waterTime)
//     : _waterLevel(waterLevel), _waterServo(waterServo), _waterFlow(waterFlow), _waterPump(waterPump), _waterTime(waterTime) {}


bool WaterTorrentManager::begin()
{
    _waterLevel.begin();
    _waterFlow.begin();
    _waterPump.begin();
    _waterServo.begin();
    _waterTime.beginTimer();
    _waterFuzzy.begin(); 


    return true;
}

void WaterTorrentManager::readingWaterLevel()
{
    waterLevel = _waterLevel.getDistance();
    userData.waterLevel = waterLevel; //Mengisi ke struct
    if (waterLevel >= 0)
    {
        ESP_LOGI(TAG, "Water Level: %.2f cm\n", waterLevel);
    }
    else
    {
        ESP_LOGW(TAG, "Water Level: No object detected");
    }
}

float WaterTorrentManager::getWaterLevel()
{
    waterLevel = _waterLevel.getDistance();
    userData.waterLevel = waterLevel; //Mengisi ke struct
    return waterLevel;
}

std::string WaterTorrentManager::RelayDriver()
{

    // _waterPump.testpump(); // Test the pump functionality
    int jsn = _waterLevel.getDistance() + 13;
    ESP_LOGI(TAG, "Current Water Level: %d cm", jsn);
    if (jsn >= 59 && pumpStatus == true) // Assuming 59 cm is the threshold for low water level
    {
        _waterPump.setPump(true); // Turn on the pump
        ESP_LOGI(TAG, "Water level low, pump ON");
        pumpStatus = false; // Update the pump status
        userData.PumpState = true; // Update the struct with pump state
        return "Pump ON"; // Return a string indicating the pump is on
    } 
    else if (jsn <= 25 && pumpStatus == false) // Assuming less than 59 cm is the threshold for high water level
    {
        _waterPump.setPump(false); // Turn off the pump
        ESP_LOGI(TAG, "Water level sufficient, pump OFF");
        pumpStatus = true; // Update the pump status
        userData.PumpState = false; // Update the struct with pump state
        return "Pump OFF"; // Return a string indicating the pump is off
    }
    else
    {
        _waterPump.setPump(pumpStatus); // Maintain the current pump state
        ESP_LOGI(TAG, "Pump state unchanged: %s", pumpStatus ? "ON" : "OFF");    
}
return std::to_string(jsn); // Return a string indicating the pump state is unchanged
}


// void WaterTorrentManager::setAngle(int angle)
// {
//     _waterServo.setAngle(angle);
//     ESP_LOGI(TAG, "Set Servo Angle: %d\n", angle);
// }



void WaterTorrentManager::readingFlowRate()
{
    flowRate = _waterFlow.getFlowRate();
    if (flowRate >= 0)
    {
        ESP_LOGI(TAG, "Flow rate: %.2f L/min\n", flowRate);
    }
    else
    {
        ESP_LOGW(TAG, "Flow rate: No object detected");
    }
}

float WaterTorrentManager::getFlowRate()
{
    flowRate = _waterFlow.getFlowRate();
    userData.flowRate = flowRate; //Mengisi ke struct
    return flowRate;
}

int WaterTorrentManager::getLitersPerDay()
{
    int daynow = _waterTime.getDay();
    if (daynow != day)
    {
        int litersPerDayNow = _waterFlow.getLiters();
        litersPerDay = litersPerDayNow - litersPerDay; // Calculate the difference in liters
        userData.LitersPerDay = litersPerDay; // Update the struct with the new liters per day
        day = daynow;
    }
    else
    {
        return -1; // Return an error message if the day has not changed
    }
    return litersPerDay; // Return the liters per day from the flow sensor
}

int WaterTorrentManager::runFuzzy()
{
    int dist = _waterLevel.getDistance();
    int daynow = _waterTime.getDay();
    // int dist = 60;
    // int daynow = 2;

    if (daynow != day)
    {
        int litersPerDayNow = _waterFlow.getLiters();
        volused = litersPerDayNow - litersPerDay; // Calculate the difference in liters
        day = daynow;
    }
    else
    {
        ESP_LOGW(TAG, "volused: No change in day, no volume used calculated");
    }

    int fuzzyout =  _waterFuzzy.run(dist, volused);
    ESP_LOGI(TAG, "Fuzzy Output: %d\n", fuzzyout);
    
    _waterServo.setAngle(fuzzyout); // Set the servo angle based on fuzzy output
    return fuzzyout; // Return the fuzzy output value
}




