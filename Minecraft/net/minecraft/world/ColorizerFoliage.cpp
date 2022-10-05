#include "ColorizerFoliage.h"

std::array<int32_t, 65536> foliageBuffer;

void ColorizerFoliage::setFoliageBiomeColorizer(const std::array<int32_t, 65536> &foliageBufferIn)
{
    foliageBuffer = foliageBufferIn;
}

int32_t ColorizerFoliage::getFoliageColor(double temperature, double humidity)
{
    humidity *= temperature;
    const uint32_t i((1.0 - temperature) * 255.0);
    const uint32_t j((1.0 - humidity) * 255.0);
    return foliageBuffer[j << 8 | i];
}

int32_t ColorizerFoliage::getFoliageColorPine()
{
    return 6396257;
}

int32_t ColorizerFoliage::getFoliageColorBirch()
{
    return 8431445;
}

int32_t ColorizerFoliage::getFoliageColorBasic()
{
    return 4764952;
}
