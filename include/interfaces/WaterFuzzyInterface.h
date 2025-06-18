#pragma once

class WaterFuzzyInterface
{
    public:
        virtual bool begin() = 0;
        virtual int run(int dist, float volused);
};