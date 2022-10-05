#pragma once
#include "Biome.h"
#include "BiomeProvider.h"

#include <cstdint>
#include <vector>

class BiomeCache
{
  public:
    struct Block
    {
        std::array<Biome *, 256> biomes;
        int32_t x;
        int32_t z;
        int64_t lastAccessTime;

        Block(int32_t x, int32_t z);
        Biome *getBiome(int32_t x, int32_t z) const;
    };

    BiomeCache(BiomeProvider providerin);
    Block getEntry(int32_t x, int32_t z);
    Biome *getBiome(int32_t x, int32_t z, Biome *defaultValue);
    void cleanupCache();
    std::array<Biome *, 256> getCachedBiomes(int32_t x, int32_t z);

  private:
    BiomeProvider provider;
    int64_t lastCleanupTime;
    std::unordered_map<int64_t, Block> cacheMap;
    std::vector<Block> cache;
};
