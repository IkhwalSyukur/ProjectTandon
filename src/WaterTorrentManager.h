#pragma once

#include <interfaces/WaterLevelSensorInterface.h>
#include <interfaces/WaterFlowSensorInterface.h>

class WaterTorrentManager
{
    public:
        WaterTorrentManager(WaterLevelSensorInterface &waterLevel, WaterFlowSensorInterface &waterFlow);
        bool begin();
        void readingWaterLevel();

    private:
        WaterLevelSensorInterface &_waterLevel;
        WaterFlowSensorInterface &_waterFlow;
};