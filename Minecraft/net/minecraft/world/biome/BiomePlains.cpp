#include "BiomePlains.h"

void BiomePlains::decorate(World* worldIn, pcg32& rand, BlockPos& pos)
{
	double d0 = GRASS_COLOR_NOISE.getValue((double)(pos.getx() + 8) / 200.0, (double)(pos.getz() + 8) / 200.0);
	if (d0 < -0.8) 
	{
		decorator.flowersPerChunk = 15;
		decorator.grassPerChunk = 5;
	}
	else 
	{
		decorator.flowersPerChunk = 4;
		decorator.grassPerChunk = 10;
		DOUBLE_PLANT_GENERATOR.setPlantType(BlockDoublePlant.EnumPlantType.GRASS);

		for (auto i1 = 0; i1 < 7; ++i1) 
		{
			auto j1 = rand(16) + 8;
			auto k1 = rand(16) + 8;
			auto l1 = rand(worldIn->getHeight(pos.add(j1, 0, k1)).getY() + 32);
			DOUBLE_PLANT_GENERATOR.generate(worldIn, rand, pos.add((int32_t)j1, l1, k1));
		}
	}

	if (sunflowers) 
	{
		DOUBLE_PLANT_GENERATOR.setPlantType(BlockDoublePlant.EnumPlantType.SUNFLOWER);

		for (auto i1 = 0; i1 < 10; ++i1) 
		{
			auto j1 = rand(16) + 8;
			auto k1 = rand(16) + 8;
			auto l1 = rand(worldIn->getHeight(pos.add(j1, 0, k1)).getY() + 32);
			DOUBLE_PLANT_GENERATOR.generate(worldIn, rand, pos.add((int32_t)j1, l1, k1));
		}
	}

	Biome::decorate(worldIn, rand, pos);
}

BiomePlains::BiomePlains(bool p_i46699_1_, BiomeProperties properties)
	:Biome(properties)
{
	sunflowers = p_i46699_1_;
	spawnableCreatureList.emplace_back(SpawnListEntry(EntityHorse.class, 5, 2, 6));
	spawnableCreatureList.emplace_back(SpawnListEntry(EntityDonkey.class, 1, 1, 3));
	decorator.treesPerChunk = 0;
	decorator.extraTreeChance = 0.05F;
	decorator.flowersPerChunk = 4;
	decorator.grassPerChunk = 10;
}

BlockFlower.EnumFlowerType Biome::pickRandomFlower(pcg32& rand, BlockPos& pos)
{
	auto d0 = GRASS_COLOR_NOISE.getValue((double)pos.getx() / 200.0, (double)pos.getz() / 200.0);
	if (d0 < -0.8) 
	{
		auto i = rand(4);
		switch (i) 
		{
		case 0:
			return BlockFlower.EnumFlowerType.ORANGE_TULIP;
		case 1:
			return BlockFlower.EnumFlowerType.RED_TULIP;
		case 2:
			return BlockFlower.EnumFlowerType.PINK_TULIP;
		case 3:
		default:
			return BlockFlower.EnumFlowerType.WHITE_TULIP;
		}
	}
	else if (rand(3) > 0) 
	{
		auto i = rand(3);
		if (i == 0) 
		{
			return BlockFlower.EnumFlowerType.POPPY;
		}
		else 
		{
			return i == 1 ? BlockFlower.EnumFlowerType.HOUSTONIA : BlockFlower.EnumFlowerType.OXEYE_DAISY;
		}
	}
	else 
	{
		return BlockFlower.EnumFlowerType.DANDELION;
	}
}

WorldGenAbstractTree Biome::getRandomTreeFeature(pcg32& rand)
{
	return (rand(3) == 0 ? Biome::BIG_TREE_FEATURE : Biome::TREE_FEATURE);
}
