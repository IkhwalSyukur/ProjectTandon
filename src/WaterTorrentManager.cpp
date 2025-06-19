#include <Arduino.h>
#include "WaterTorrentManager.h"
#include "esp_log.h"
#include <string>
#include "WaterTorrentPubSource.h"

const char* MQTT_SERVER = "test.mosquitto.org"; // Public MQTT broker
const int MQTT_PORT = 1883;
const char* MQTT_ID = "WaterTorrentManager";
const char* MQTT_PUBLISH_TOPIC = "water_torrent/sensorData";
const char* MQTT_SUBSCRIBE_TOPIC = "water_torrent_armisuari/control";

static const char *TAG = "WaterTorrentManager";
WaterTorrentManager* WaterTorrentManager::instance = nullptr;

bool WaterTorrentManager::begin()
{
    instance = this; // Set the singleton instance
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
    _waterLevel.begin();
    _waterFlow.begin();
    _waterPump.begin();
    _waterServo.begin();
    _waterTime.beginTimer();
    _waterFuzzy.begin(); 

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
    
    int waterLevel = instance->userData.waterLevel; // Access waterLevel from the instance's userData
    float waterVolume = instance->userData.volumeUsed; // Access flowRate from the instance's userData
    bool waterpumpStatus = instance->userData.PumpState; // Access pump state from the instance's userData
    std::string waterLevelStatus = instance->userData.waterlevelstatus; // Access water level status from the instance's userData
    std::string waterVolumeStatus = instance->userData.watervolumestatus; // Access water volume status from the instance's userData    
    std::string waterServoStatus = instance->userData.waterservostatus; // Access water servo status from the instance's userData

    // doc["sensor_id"] = MQTT_ID + std::string("_") + instance->_wifiAdapter->getMacAddress();
    doc["sensor_id"] = 1;
    doc["ketinggian_air"] = waterLevel;
    doc["volume_air"] = waterVolume;
    doc["status_pompa"] = waterpumpStatus;
    doc["status_ketinggia_air"] = waterLevelStatus;
    doc["status_volume_air"] = waterVolumeStatus;
    doc["status_laju_air"] = waterServoStatus;
    doc["timestamp"] = time(nullptr); // Current time in seconds since epoch
}


float WaterTorrentManager::getWaterLevel()
{
    waterLevel = _waterLevel.getDistance();
    userData.waterLevel = waterLevel; //Mengisi ke struct
    if (waterLevel <= 0)
    {
        ESP_LOGE(TAG, "water level sensor error: %f cm", waterLevel);
        return false;
    }

    return waterLevel; // Return the water level from the sensor
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
    flowRate = _waterFlow.getLiters();
    userData.volumeUsed = flowRate; //Mengisi ke struct
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
    float volused = _waterFlow.getLiters();
    int daynow = _waterTime.getDay();
    
    if (dist <= 35){
        userData.waterlevelstatus = "Tinggi";
    }
    else if (dist > 35 && dist <= 50){
        userData.waterlevelstatus = "Sedang";
    }
    else {
        userData.waterlevelstatus = "Rendah";
    }
    
    if (volused <= 40){
        userData.watervolumestatus = "Sedikit";
    }
    else if (volused > 40 && volused <= 80){
        userData.watervolumestatus = "Normal";
    }
    else {
        userData.watervolumestatus = "Boros";
    }

    // int dist = 60;
    // int daynow = 2;

    if (daynow != 1)
    {
        int litersPerDayNow = _waterFlow.getLiters();
        volused = litersPerDayNow - litersPerDay; // Calculate the difference in liters
        day = daynow;
    }
    else
    {
        ESP_LOGW(TAG, "volused: No change in day, no volume used calculated");
    }
    
    int fuzzy_data = _waterFuzzy.runFuzzy(dist, volused);
    ESP_LOGI(TAG, "Fuzzy Output: %d", fuzzy_data);

    _waterServo.setAngle(fuzzy_data); // Set servo angle based on fuzzy output

    // Optionally, you can use fuzzy_data to determine waterservostatus
    if (fuzzy_data <= 80){
        userData.waterservostatus = "Cepat";
    }
    else if (fuzzy_data > 80 && fuzzy_data <= 120){
        userData.waterservostatus = "Sedang";
    }
    else {
        userData.waterservostatus = "Lambat";
    }

    return fuzzy_data; // Return the fuzzy output
}