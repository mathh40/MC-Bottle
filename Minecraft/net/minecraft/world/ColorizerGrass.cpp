#include "ColorizerGrass.h"

std::array<int32_t, 65536> grassBuffer;

void ColorizerGrass::setGrassBiomeColorizer(const std::array<int32_t, 65536> &grassBufferIn)
{
    grassBuffer = grassBufferIn;
}

int32_t ColorizerGrass::getGrassColor(double temperature, double humidity)
{
    humidity *= temperature;
    uint32_t i((1.0 - temperature) * 255.0);
    uint32_t j((1.0 - humidity) * 255.0);
    uint32_t k = j << 8 | i;
    return k > grassBuffer.max_size() ? -65281 : grassBuffer[k];
}
