#include "BiomeSnow.h"

BiomeSnow::BiomeSnow(bool superIcyIn, BiomeProperties properties)
	:Biome(properties)
{
	superIcy = superIcyIn;
	if (superIcyIn) 
	{
		topBlock = Blocks.SNOW.getDefaultState();
	}

	spawnableCreatureList.clear();
	spawnableCreatureList.emplace_back(SpawnListEntry(EntityRabbit.class, 10, 2, 3));
	spawnableCreatureList.emplace_back(SpawnListEntry(EntityPolarBear.class, 1, 1, 2));


	auto iterator = spawnableMonsterList.begin();
	while (iterator != spawnableMonsterList.end())
	{
		if (*iterator.entityClass == EntitySkeleton.class)
		{
			iterator = spawnableMonsterList.erase(iterator);
		}
	}

	spawnableMonsterList.emplace_back(SpawnListEntry(EntitySkeleton.class, 20, 4, 4));
	spawnableMonsterList.emplace_back(SpawnListEntry(EntityStray.class, 80, 4, 4));
}

float BiomeSnow::getSpawningChance()
{
	return 0.07F;
}

void BiomeSnow::decorate(World* worldIn, pcg32& rand, BlockPos& pos)
{
	if (superIcy) 
	{
		for (auto l = 0; l < 3; ++l) 
		{
			auto i1 = rand(16) + 8;
			auto j1 = rand(16) + 8;
			iceSpike.generate(worldIn, rand, worldIn->getHeight(pos.add(i1, 0, j1)));
		}

		for (auto l = 0; l < 2; ++l)
		{
			auto i1 = rand(16) + 8;
			auto j1 = rand(16) + 8;
			icePatch.generate(worldIn, rand, worldIn->getHeight(pos.add(i1, 0, j1)));
		}
	}

	Biome::decorate(worldIn, rand, pos);
}

WorldGenAbstractTree BiomeSnow::getRandomTreeFeature(pcg32& rand)
{
	return new WorldGenTaiga2(false);
}
