#include "BiomeVoid.h"

BiomeVoid::BiomeVoid(BiomeProperties properties) : Biome(properties)
{
    spawnableMonsterList.clear();
    spawnableCreatureList.clear();
    spawnableWaterCreatureList.clear();
    spawnableCaveCreatureList.clear();
    decorator = new BiomeVoidDecorator();
}

bool BiomeVoid::ignorePlayerSpawnSuitability() const
{
    return true;
}
