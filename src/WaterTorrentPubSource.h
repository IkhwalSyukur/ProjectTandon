#pragma once

typedef struct
{
    int waterLevel;
    float volumeUsed;
    bool PumpState;
    int LitersPerDay;
    std::string waterlevelstatus;
    std::string watervolumestatus;
    std::string waterservostatus;
    int TelegramTrig; // 1 = water too low, pump on :  2 = water full, pump off
}WaterTorrentData_t;
