#pragma once
#include <Arduino.h>
#include <interfaces/WaterFuzzyInterface.h>
#include <Fuzzy.h>

class WaterFuzzy : public WaterFuzzyInterface
{
    public:

        bool begin() override;
        int run(int dist, float volused) override;

};