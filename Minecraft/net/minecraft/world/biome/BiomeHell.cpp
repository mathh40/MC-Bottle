#include "BiomeHell.h"

#include "BiomeHellDecorator.h"

BiomeHell::BiomeHell(BiomeProperties properties) : Biome(properties)
{
    spawnableMonsterList.clear();
    spawnableCreatureList.clear();
    spawnableWaterCreatureList.clear();
    spawnableCaveCreatureList.clear();
    spawnableMonsterList.emplace_back(SpawnListEntry(EntityGhast.class, 50, 4, 4));
    spawnableMonsterList.emplace_back(SpawnListEntry(EntityPigZombie.class, 100, 4, 4));
    spawnableMonsterList.emplace_back(SpawnListEntry(EntityMagmaCube.class, 2, 4, 4));
    spawnableMonsterList.emplace_back(SpawnListEntry(EntityEnderman.class, 1, 4, 4));
    decorator = new BiomeHellDecorator();
}
