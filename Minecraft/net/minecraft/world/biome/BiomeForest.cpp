#include "BiomeForest.h"
#include "../../util/math/MathHelper.h"

BiomeForest::BiomeForest(Type typeIn, BiomeProperties properties)
	:Biome(properties)
{
	type = typeIn;
	decorator.treesPerChunk = 10;
	decorator.grassPerChunk = 2;
	if (type == Type::FLOWER) 
	{
		decorator.treesPerChunk = 6;
		decorator.flowersPerChunk = 100;
		decorator.grassPerChunk = 1;
		spawnableCreatureList.emplace_back(SpawnListEntry(EntityRabbit.class, 4, 2, 3));
	}

	if (type == Type::NORMAL) 
	{
		spawnableCreatureList.emplace_back(SpawnListEntry(EntityWolf.class, 5, 4, 4));
	}

	if (type == Type::ROOFED) 
	{
		decorator.treesPerChunk = -999;
	}
}

WorldGenAbstractTree BiomeForest::getRandomTreeFeature(pcg32& rand)
{
	if (type == Type::ROOFED && rand(3) > 0)
	{
		return ROOF_TREE;
	}
	else if (type != Type::BIRCH && rand(5) != 0) 
	{
		return (WorldGenAbstractTree)(rand(10) == 0 ? BIG_TREE_FEATURE : TREE_FEATURE);
	}
	else {
		return BIRCH_TREE;
	}
}

BlockFlower.EnumFlowerType BiomeForest::pickRandomFlower(pcg32& rand, BlockPos& pos)
{
	if (type == Type::FLOWER) 
	{
		auto d0 = MathHelper::clamp((1.0 + GRASS_COLOR_NOISE.getValue((double)pos.getx() / 48.0, (double)pos.getz() / 48.0)) / 2.0, 0.0, 0.9999);
		BlockFlower.EnumFlowerType blockflower$enumflowertype = BlockFlower.EnumFlowerType.values()[(int)(d0 * (double)BlockFlower.EnumFlowerType.values().length)];
		return blockflower$enumflowertype == BlockFlower.EnumFlowerType.BLUE_ORCHID ? BlockFlower.EnumFlowerType.POPPY : blockflower$enumflowertype;
	}
	else 
	{
		return Biome::pickRandomFlower(rand, pos);
	}
}

void BiomeForest::decorate(World* worldIn, pcg32& rand, BlockPos& pos)
{
	if (type == Type::ROOFED) 
	{
		addMushrooms(worldIn, rand, pos);
	}

	int i = rand(5) - 3;
	if (type == Type::FLOWER) 
	{
		i += 2;
	}

	addDoublePlants(worldIn, rand, pos, i);
	Biome::decorate(worldIn, rand, pos);
}

int BiomeForest::getGrassColorAtPos(BlockPos& pos)
{
	auto i = Biome::getGrassColorAtPos(pos);
	return type == Type::ROOFED ? (i & 16711422) + 2634762 >> 1 : i;
}

void BiomeForest::addMushrooms(World* p_185379_1_, pcg32& p_185379_2_, BlockPos& p_185379_3_)
{
	for (auto i = 0; i < 4; ++i) 
	{
		for (auto j = 0; j < 4; ++j)
		{
			auto k = i * 4 + 1 + 8 + p_185379_2_(3);
			auto l = j * 4 + 1 + 8 + p_185379_2_(3);
			BlockPos blockpos = p_185379_1_->getHeight(p_185379_3_.add(k, 0, l));
			if (p_185379_2_(20) == 0) 
			{
				WorldGenBigMushroom worldgenbigmushroom = new WorldGenBigMushroom();
				worldgenbigmushroom.generate(p_185379_1_, p_185379_2_, blockpos);
			}
			else 
			{
				WorldGenAbstractTree worldgenabstracttree = this.getRandomTreeFeature(p_185379_2_);
				worldgenabstracttree.setDecorationDefaults();
				if (worldgenabstracttree.generate(p_185379_1_, p_185379_2_, blockpos)) {
					worldgenabstracttree.generateSaplings(p_185379_1_, p_185379_2_, blockpos);
				}
			}
		}
	}
}

void BiomeForest::addDoublePlants(World* p_185378_1_, pcg32& p_185378_2_, BlockPos& p_185378_3_, int32_t p_185378_4_)
{
	for (auto i = 0; i < p_185378_4_; ++i) 
	{
		auto j = p_185378_2_(3);
		if (j == 0) 
		{
			DOUBLE_PLANT_GENERATOR.setPlantType(BlockDoublePlant.EnumPlantType.SYRINGA);
		}
		else if (j == 1) 
		{
			DOUBLE_PLANT_GENERATOR.setPlantType(BlockDoublePlant.EnumPlantType.ROSE);
		}
		else if (j == 2) 
		{
			DOUBLE_PLANT_GENERATOR.setPlantType(BlockDoublePlant.EnumPlantType.PAEONIA);
		}

		for (int k = 0; k < 5; ++k) 
		{
			int l = p_185378_2_(16) + 8;
			int i1 = p_185378_2_(16) + 8;
			int j1 = p_185378_2_(p_185378_1_->getHeight(p_185378_3_.add(l, 0, i1)).getY() + 32);
			if (DOUBLE_PLANT_GENERATOR.generate(p_185378_1_, p_185378_2_, BlockPos(p_185378_3_.getx() + l, j1, p_185378_3_.getz() + i1)))
			{
				break;
			}
		}
	}
}
