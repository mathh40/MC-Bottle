#include "BiomeCache.h"

BiomeCache::BiomeCache(BiomeProvider providerin) : provider(providerin)
{
    cacheMap.reserve(4096);
}

Biome *BiomeCache::getBiome(int32_t x, int32_t z, Biome *defaultValue)
{
    auto biome = getEntry(x, z).getBiome(x, z);
    return biome == nullptr ? defaultValue : biome;
}

void BiomeCache::cleanupCache()
{
    auto i = MinecraftServer::getCurrentTimeMillis();
    auto j = i - lastCleanupTime;
    if (j > 7500 || j < 0)
    {
        lastCleanupTime = i;

        for (auto k = 0; k < cache.size(); ++k)
        {
            auto biomecache$block = cache[k];
            auto l                = i - biomecache$block.lastAccessTime;
            if (l > 30000 || l < 0)
            {
                cache.erase(cache.begin() + k--);
                auto i1 = biomecache$block.x & 4294967295 | (biomecache$block.z & 4294967295) << 32;
                cacheMap.erase(cacheMap.begin() + i1);
            }
        }
    }
}

BiomeCache::Block::Block(int32_t x, int32_t z) : x(x), z(z)
{
    provider.getBiomes(biomes, x << 4, z << 4, 16, 16, false);
}

Biome *BiomeCache::Block::getBiome(int32_t x, int32_t z) const
{
    return biomes[x & 15 | (z & 15) << 4];
}

std::array<Biome *, 256> BiomeCache::getCachedBiomes(int32_t x, int32_t z)
{
    return getEntry(x, z).biomes;
}

BiomeCache::Block BiomeCache::getEntry(int32_t x, int32_t z)
{
    x >>= 4;
    z >>= 4;
    auto i                = x & 4294967295 | (z & 4294967295) << 32;
    auto biomecache$block = cacheMap[i];
    if (biomecache$block == nullptr)
    {
        biomecache$block = Block(x, z);
        cacheMap.emplace(i, biomecache$block);
        cache.emplace_back(biomecache$block);
    }

    biomecache$block.lastAccessTime = MinecraftServer.getCurrentTimeMillis();
    return biomecache$block;
}
