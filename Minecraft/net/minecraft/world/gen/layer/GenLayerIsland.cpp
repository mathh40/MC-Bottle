#include "GenLayerIsland.h"

#include "IntCache.h"

GenLayerIsland::GenLayerIsland(int64_t p_i2124_1_) : GenLayer(p_i2124_1_)
{
}

std::vector<int32_t> GenLayerIsland::getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight)
{
    auto aint = IntCache::getIntCache(areaWidth * areaHeight);

    for (auto i = 0; i < areaHeight; ++i)
    {
        for (auto j = 0; j < areaWidth; ++j)
        {
            initChunkSeed((int64_t)(areaX + j), (int64_t)(areaY + i));
            aint[j + i * areaWidth] = nextInt(10) == 0 ? 1 : 0;
        }
    }

    if (areaX > -areaWidth && areaX <= 0 && areaY > -areaHeight && areaY <= 0)
    {
        aint[-areaX + -areaY * areaWidth] = 1;
    }

    return aint;
}
