#include "BiomeOcean.h"

BiomeOcean::BiomeOcean(BiomeProperties properties) : Biome(properties)
{
    spawnableCreatureList.clear();
}

TempCategory BiomeOcean::getTempCategory()
{
    return TempCategory::OCEAN;
}
