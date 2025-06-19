#pragma once

class WaterFuzzyInterface
{
    public:
        virtual bool begin() = 0;
        virtual int runFuzzy(int dist, float volused);
};