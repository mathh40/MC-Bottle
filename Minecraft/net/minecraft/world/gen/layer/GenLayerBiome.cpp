#include "GenLayerBiome.h"

#include "IntCache.h"
#include "WorldType.h"

GenLayerBiome::GenLayerBiome(int64_t p_i45560_1_, std::shared_ptr<GenLayer> p_i45560_3_, WorldType p_i45560_4_,
                             ChunkGeneratorSettings p_i45560_5_)
    : GenLayer(p_i45560_1_)
{
    auto warmBiomes   = {Biomes::DESERT,  Biomes::DESERT,  Biomes::DESERT,
                         Biomes::SAVANNA, Biomes::SAVANNA, Biomes::PLAINS};
    auto mediumBiomes = {Biomes::FOREST, Biomes::ROOFED_FOREST, Biomes::EXTREME_HILLS,
                         Biomes::PLAINS, Biomes::BIRCH_FOREST,  Biomes::SWAMPLAND};
    auto coldBiomes   = {Biomes::FOREST, Biomes::EXTREME_HILLS, Biomes::TAIGA, Biomes::PLAINS};
    auto iceBiomes    = {Biomes::ICE_PLAINS, Biomes::ICE_PLAINS, Biomes::ICE_PLAINS, Biomes::COLD_TAIGA};
    parent            = p_i45560_3_;
    if (p_i45560_4_ == WorldType::DEFAULT_1_1)
    {
        warmBiomes = {Biomes::DESERT,    Biomes::FOREST, Biomes::EXTREME_HILLS,
                      Biomes::SWAMPLAND, Biomes::PLAINS, Biomes::TAIGA};
        settings   = std::nullopt;
    }
    else
    {
        settings = p_i45560_5_;
    }
}

std::vector<int32_t> GenLayerBiome::getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight)
{
    auto aint  = parent->getInts(areaX, areaY, areaWidth, areaHeight);
    auto aint1 = IntCache::getIntCache(areaWidth * areaHeight);

    for (auto i = 0; i < areaHeight; ++i)
    {
        for (auto j = 0; j < areaWidth; ++j)
        {
            initChunkSeed((int64_t)(j + areaX), (int64_t)(i + areaY));
            auto k = aint[j + i * areaWidth];
            auto l = (k & 3840) >> 8;
            k &= -3841;
            if (settings.has_value() && settings.value().fixedBiome >= 0)
            {
                aint1[j + i * areaWidth] = settings.value().fixedBiome;
            }
            else if (isBiomeOceanic(k))
            {
                aint1[j + i * areaWidth] = k;
            }
            else if (k == Biome::getIdForBiome(Biomes::MUSHROOM_ISLAND))
            {
                aint1[j + i * areaWidth] = k;
            }
            else if (k == 1)
            {
                if (l > 0)
                {
                    if (nextInt(3) == 0)
                    {
                        aint1[j + i * areaWidth] = Biome::getIdForBiome(Biomes::MESA_CLEAR_ROCK);
                    }
                    else
                    {
                        aint1[j + i * areaWidth] = Biome::getIdForBiome(Biomes::MESA_ROCK);
                    }
                }
                else
                {
                    aint1[j + i * areaWidth] = Biome::getIdForBiome(warmBiomes[nextInt(warmBiomes.size())]);
                }
            }
            else if (k == 2)
            {
                if (l > 0)
                {
                    aint1[j + i * areaWidth] = Biome::getIdForBiome(Biomes::JUNGLE);
                }
                else
                {
                    aint1[j + i * areaWidth] = Biome::getIdForBiome(mediumBiomes[nextInt(mediumBiomes.size())]);
                }
            }
            else if (k == 3)
            {
                if (l > 0)
                {
                    aint1[j + i * areaWidth] = Biome::getIdForBiome(Biomes::REDWOOD_TAIGA);
                }
                else
                {
                    aint1[j + i * areaWidth] = Biome::getIdForBiome(coldBiomes[nextInt(coldBiomes.size())]);
                }
            }
            else if (k == 4)
            {
                aint1[j + i * areaWidth] = Biome::getIdForBiome(iceBiomes[nextInt(iceBiomes.size())]);
            }
            else
            {
                aint1[j + i * areaWidth] = Biome::getIdForBiome(Biomes::MUSHROOM_ISLAND);
            }
        }
    }

    return aint1;
}
