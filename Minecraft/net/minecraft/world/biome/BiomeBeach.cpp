#include "BiomeBeach.h"

BiomeBeach::BiomeBeach(BiomeProperties properties) : Biome(properties)
{
    spawnableCreatureList.clear();
    topBlock                   = Blocks::SAND.getDefaultState();
    fillerBlock                = Blocks::SAND.getDefaultState();
    decorator.treesPerChunk    = -999;
    decorator.deadBushPerChunk = 0;
    decorator.reedsPerChunk    = 0;
    decorator.cactiPerChunk    = 0;
}
