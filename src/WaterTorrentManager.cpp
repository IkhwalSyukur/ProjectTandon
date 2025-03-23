#include "WaterTorrentManager.h"
#include "esp_log.h"

static const char *TAG = "WaterTorrentManager";

WaterTorrentManager::WaterTorrentManager(WaterLevelSensorInterface &waterLevel, WaterFlowSensorInterface &waterFlow)
: _waterLevel(waterLevel), _waterFlow(waterFlow)
{
}

bool WaterTorrentManager::begin()
{
    return _waterLevel.begin();
}

void WaterTorrentManager::readingWaterLevel()
{
    float waterLevel = _waterLevel.getDistance();
    if (waterLevel >= 0)
    {
        ESP_LOGI(TAG, "Water Level: %.2f cm\n", waterLevel);
    }
    else
    {
        ESP_LOGW(TAG, "Water Level: No object detected");
    }
}