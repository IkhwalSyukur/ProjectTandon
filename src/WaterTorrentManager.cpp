#include <Arduino.h>
#include "WaterTorrentManager.h"
#include "esp_log.h"
#include <string>

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
}