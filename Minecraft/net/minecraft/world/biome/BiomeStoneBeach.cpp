#include "BiomeStoneBeach.h"

BiomeStoneBeach::BiomeStoneBeach(BiomeProperties properties) : Biome(properties)
{
    spawnableCreatureList.clear();
    topBlock                   = Blocks.STONE.getDefaultState();
    fillerBlock                = Blocks.STONE.getDefaultState();
    decorator.treesPerChunk    = -999;
    decorator.deadBushPerChunk = 0;
    decorator.reedsPerChunk    = 0;
    decorator.cactiPerChunk    = 0;
}
