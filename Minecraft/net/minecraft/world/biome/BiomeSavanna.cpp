#include "BiomeSavanna.h"

BiomeSavanna::BiomeSavanna(BiomeProperties properties)
	:Biome(properties)
{
	spawnableCreatureList.emplace_back(SpawnListEntry(EntityHorse.class, 1, 2, 6));
	spawnableCreatureList.emplace_back(SpawnListEntry(EntityDonkey.class, 1, 1, 1));
	if (getBaseHeight() > 1.1F) 
	{
		spawnableCreatureList.emplace_back(SpawnListEntry(EntityLlama.class, 8, 4, 4));
	}

	decorator.treesPerChunk = 1;
	decorator.flowersPerChunk = 4;
	decorator.grassPerChunk = 20;
}

WorldGenAbstractTree BiomeSavanna::getRandomTreeFeature(pcg32& rand)
{
	return (WorldGenAbstractTree)(rand(5) > 0 ? SAVANNA_TREE : TREE_FEATURE);
}

void BiomeSavanna::decorate(World* worldIn, pcg32& rand, BlockPos& pos)
{
	DOUBLE_PLANT_GENERATOR.setPlantType(BlockDoublePlant.EnumPlantType.GRASS);

	for (int i = 0; i < 7; ++i) {
		int j = rand(16) + 8;
		int k = rand(16) + 8;
		int l = rand(worldIn->getHeight(pos.add(j, 0, k)).getY() + 32);
		DOUBLE_PLANT_GENERATOR.generate(worldIn, rand, pos.add(j, l, k));
	}

	BiomeSavanna::decorate(worldIn, rand, pos);
}
