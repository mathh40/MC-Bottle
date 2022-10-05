#include "BiomeMushroomIsland.h"

BiomeMushroomIsland::BiomeMushroomIsland(BiomeProperties properties) : Biome(properties)
{
    decorator.treesPerChunk        = -100;
    decorator.flowersPerChunk      = -100;
    decorator.grassPerChunk        = -100;
    decorator.mushroomsPerChunk    = 1;
    decorator.bigMushroomsPerChunk = 1;
    topBlock                       = Blocks.MYCELIUM.getDefaultState();
    spawnableMonsterList.clear();
    spawnableCreatureList.clear();
    spawnableWaterCreatureList.clear();
    spawnableCreatureList.emplace_back(SpawnListEntry(EntityMooshroom.class, 8, 4, 8));
}
