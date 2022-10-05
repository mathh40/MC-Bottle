#include "BiomeProviderSingle.h"

BiomeProviderSingle::BiomeProviderSingle(Biome *biomeIn) : biome(biomeIn)
{
}

Biome *BiomeProviderSingle::getBiome(BlockPos &pos) const
{
    return biome;
}

std::vector<Biome *> BiomeProviderSingle::getBiomesForGeneration(std::vector<Biome *> biomes, int32_t x, int32_t z,
                                                                 int32_t width, int32_t height)
{
    if (biomes.empty() || biomes.size() < width * height)
    {
        biomes.resize(width * height);
    }

    std::fill(biomes.begin(), biomes.end(), biome);
    return biomes;
}

std::vector<Biome *> BiomeProviderSingle::getBiomes(std::vector<Biome *> oldBiomeList, int32_t x, int32_t z,
                                                    int32_t width, int32_t depth)
{
    if (oldBiomeList.empty() || oldBiomeList.size() < width * depth)
    {
        oldBiomeList.resize(width * depth);
    }

    std::fill(oldBiomeList.begin(), oldBiomeList.end(), biome);
    return oldBiomeList;
}

std::vector<Biome *> BiomeProviderSingle::getBiomes(std::vector<Biome *> listToReuse, int32_t x, int32_t z,
                                                    int32_t width, int32_t length, bool cacheFlag)
{
    return getBiomes(listToReuse, x, z, width, length);
}

std::optional<BlockPos> BiomeProviderSingle::findBiomePosition(int32_t x, int32_t z, int32_t range,
                                                               std::vector<Biome *> biomes, pcg32 &random)
{
    auto ite = std::find_if(biomes.begin(), biomes.end(), [&](const Biome *lhs) { return lhs == biome; });
    return ite != biomes.end() ? BlockPos(x - range + random(range * 2 + 1), 0, z - range + random(range * 2 + 1))
                               : std::nullopt;
}

bool BiomeProviderSingle::areBiomesViable(int32_t x, int32_t z, int32_t radius, std::vector<Biome *> allowed)
{
    auto ite = std::find_if(allowed.begin(), allowed.end(), [&](const Biome *lhs) { return lhs == biome; });
    return ite != allowed.end();
}

bool BiomeProviderSingle::isFixedBiome()
{
    return true;
}

Biome *BiomeProviderSingle::getFixedBiome()
{
    return biome;
}
