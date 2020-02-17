#include "BiomeEnd.h"

BiomeEnd::BiomeEnd(BiomeProperties properties)
	:Biome(properties)
{
	spawnableMonsterList.clear();
	spawnableCreatureList.clear();
	spawnableWaterCreatureList.clear();
	spawnableCaveCreatureList.clear();
	spawnableMonsterList.emplace_back(SpawnListEntry(EntityEnderman.class, 10, 4, 4));
	topBlock = Blocks.DIRT.getDefaultState();
	fillerBlock = Blocks.DIRT.getDefaultState();
	decorator = new BiomeEndDecorator();
}

int32_t BiomeEnd::getSkyColorByTemp(float currentTemperature)
{
	return 0;
}
