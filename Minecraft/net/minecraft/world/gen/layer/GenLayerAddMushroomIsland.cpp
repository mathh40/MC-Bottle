#include "GenLayerAddMushroomIsland.h"

#include "IntCache.h"
#include "biome/Biome.h"

GenLayerAddMushroomIsland::GenLayerAddMushroomIsland(int64_t p_i2120_1_, std::shared_ptr<GenLayer> p_i2120_3_)
    : GenLayer(p_i2120_1_)
{
    parent = p_i2120_3_;
}

std::vector<int32_t> GenLayerAddMushroomIsland::getInts(int32_t areaX, int32_t areaY, int32_t areaWidth,
                                                        int32_t areaHeight)
{
    auto i     = areaX - 1;
    auto j     = areaY - 1;
    auto k     = areaWidth + 2;
    auto l     = areaHeight + 2;
    auto aint  = parent->getInts(i, j, k, l);
    auto aint1 = IntCache::getIntCache(areaWidth * areaHeight);

    for (auto i1 = 0; i1 < areaHeight; ++i1)
    {
        for (auto j1 = 0; j1 < areaWidth; ++j1)
        {
            auto k1 = aint[j1 + 0 + (i1 + 0) * k];
            auto l1 = aint[j1 + 2 + (i1 + 0) * k];
            auto i2 = aint[j1 + 0 + (i1 + 2) * k];
            auto j2 = aint[j1 + 2 + (i1 + 2) * k];
            auto k2 = aint[j1 + 1 + (i1 + 1) * k];
            initChunkSeed((int32_t)(j1 + areaX), (int32_t)(i1 + areaY));
            if (k2 == 0 && k1 == 0 && l1 == 0 && i2 == 0 && j2 == 0 && nextInt(100) == 0)
            {
                aint1[j1 + i1 * areaWidth] = Biome::getIdForBiome(Biomes::MUSHROOM_ISLAND);
            }
            else
            {
                aint1[j1 + i1 * areaWidth] = k2;
            }
        }
    }

    return aint1;
}
