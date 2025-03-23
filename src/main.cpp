#include <Arduino.h>

#include <WaterTorrentManager.h>
#include <driver/WaterLevel_JSN04T.h>

WaterLevel_JSN04T waterLevel(12, 14);

WaterTorrentManager waterTorrent(waterLevel);

void setup()
{
  Serial.begin(115200);
  waterTorrent.begin();
}

void loop()
{
  waterTorrent.readingWaterLevel();
  delay(1000);
}
