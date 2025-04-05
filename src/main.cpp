#include <Arduino.h>
#include <memory>
#include "esp_log.h"

#include <driver/WifiAdapter_ESP.h>
#include <WaterTorrentManager.h>

std::unique_ptr<WifiAdapterESP> wifiAdapter = std::make_unique<WifiAdapterESP>();
WaterTorrentManager waterTorrentManager(std::move(wifiAdapter));

static const char *TAG = "main";

void setup()
{
  Serial.begin(115200);
  vTaskDelay(1000); // Give time for the serial monitor to open

  ESP_LOGD(TAG, "Initializing WaterTorrentManager...");
  if (!waterTorrentManager.begin())
  {
    ESP_LOGE(TAG, "Failed to initialize WaterTorrentManager.");
    return;
  }

  // Add any additional setup code here
  ESP_LOGI(TAG, "WaterTorrentManager setup complete.");
}

void loop()
{
  vTaskDelete(NULL); // Delete the loop task to save power
                     // The main logic of your program will be handled in the setup function or in other tasks
                     // You can add any additional code here if needed
}
