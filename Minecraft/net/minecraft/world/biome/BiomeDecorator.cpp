#include "BiomeDecorator.h"
#include <random>
#include "../../block/material/Material.h"

BiomeDecorator::BiomeDecorator()
{
	sandGen = new WorldGenSand(Blocks.SAND, 7);
	gravelGen = new WorldGenSand(Blocks.GRAVEL, 6);
	flowerGen = new WorldGenFlowers(Blocks.YELLOW_FLOWER, BlockFlower.EnumFlowerType.DANDELION);
	mushroomBrownGen = new WorldGenBush(Blocks.BROWN_MUSHROOM);
	mushroomRedGen = new WorldGenBush(Blocks.RED_MUSHROOM);
	bigMushroomGen = new WorldGenBigMushroom();
	reedGen = new WorldGenReed();
	cactusGen = new WorldGenCactus();
	waterlilyGen = new WorldGenWaterlily();
	extraTreeChance = 0.1F;
	flowersPerChunk = 2;
	grassPerChunk = 1;
	gravelPatchesPerChunk = 1;
	sandPatchesPerChunk = 3;
	clayPerChunk = 1;
	generateFalls = true;
}

void BiomeDecorator::decorate(World* worldIn, pcg32& random, Biome* biome, BlockPos& pos)
{
	if (decorating) 
	{
		throw std::runtime_error("Already decorating");
	}
	else 
	{
		chunkProviderSettings = ChunkGeneratorSettings.Factory.jsonToFactory(worldIn->getWorldInfo().getGeneratorOptions()).build();
		chunkPos = pos;
		dirtGen = new WorldGenMinable(Blocks.DIRT.getDefaultState(), chunkProviderSettings.dirtSize);
		gravelOreGen = new WorldGenMinable(Blocks.GRAVEL.getDefaultState(), chunkProviderSettings.gravelSize);
		graniteGen = new WorldGenMinable(Blocks.STONE.getDefaultState().withProperty(BlockStone.VARIANT, BlockStone.EnumType.GRANITE), chunkProviderSettings.graniteSize);
		dioriteGen = new WorldGenMinable(Blocks.STONE.getDefaultState().withProperty(BlockStone.VARIANT, BlockStone.EnumType.DIORITE), chunkProviderSettings.dioriteSize);
		andesiteGen = new WorldGenMinable(Blocks.STONE.getDefaultState().withProperty(BlockStone.VARIANT, BlockStone.EnumType.ANDESITE), chunkProviderSettings.andesiteSize);
		coalGen = new WorldGenMinable(Blocks.COAL_ORE.getDefaultState(), chunkProviderSettings.coalSize);
		ironGen = new WorldGenMinable(Blocks.IRON_ORE.getDefaultState(), chunkProviderSettings.ironSize);
		goldGen = new WorldGenMinable(Blocks.GOLD_ORE.getDefaultState(), chunkProviderSettings.goldSize);
		redstoneGen = new WorldGenMinable(Blocks.REDSTONE_ORE.getDefaultState(), chunkProviderSettings.redstoneSize);
		diamondGen = new WorldGenMinable(Blocks.DIAMOND_ORE.getDefaultState(), chunkProviderSettings.diamondSize);
		lapisGen = new WorldGenMinable(Blocks.LAPIS_ORE.getDefaultState(), chunkProviderSettings.lapisSize);
		genDecorations(biome, worldIn, random);
		decorating = false;
	}
}

void BiomeDecorator::genDecorations(Biome* biomeIn, World* worldIn, pcg32& random)
{
	generateOres(worldIn, random);

	for (auto k1 = 0; k1 < sandPatchesPerChunk; ++k1) 
	{
		auto l5 = random(16) + 8;
		auto j10 = random(16) + 8;
		sandGen.generate(worldIn, random, worldIn->getTopSolidOrLiquidBlock(chunkPos.add(l5, 0, j10)));
	}

	for (auto k1 = 0; k1 < clayPerChunk; ++k1) 
	{
		auto l5 = random(16) + 8;
		auto j10 = random(16) + 8;
		clayGen.generate(worldIn, random, worldIn->getTopSolidOrLiquidBlock(chunkPos.add(l5, 0, j10)));
	}

	for (auto k1 = 0; k1 < gravelPatchesPerChunk; ++k1) 
	{
		auto l5 = random(16) + 8;
		auto j10 = random(16) + 8;
		gravelGen.generate(worldIn, random, worldIn->getTopSolidOrLiquidBlock(chunkPos.add(l5, 0, j10)));
	}

	auto k1 = treesPerChunk;
	std::uniform_real_distribution<double> dis(0.0, 1.0);
	if (dis(random) < extraTreeChance)
	{
		++k1;
	}

	for (auto l5 = 0; l5 < k1; ++l5) 
	{
		auto j10 = random(16) + 8;
		auto i14 = random(16) + 8;
		WorldGenAbstractTree worldgenabstracttree = biomeIn->getRandomTreeFeature(random);
		worldgenabstracttree.setDecorationDefaults();
		auto blockpos3 = worldIn->getHeight(chunkPos.add(j10, 0, i14));
		if (worldgenabstracttree.generate(worldIn, random, blockpos3)) 
		{
			worldgenabstracttree.generateSaplings(worldIn, random, blockpos3);
		}
	}

	for (auto l5 = 0; l5 < bigMushroomsPerChunk; ++l5) 
	{
		auto j10 = random(16) + 8;
		auto i14 = random(16) + 8;
		bigMushroomGen.generate(worldIn, random, worldIn->getHeight(chunkPos.add(j10, 0, i14)));
	}

	for (auto l5 = 0; l5 < flowersPerChunk; ++l5)
	{
		auto j10 = random(16) + 8;
		auto i14 = random(16) + 8;
		auto j17 = worldIn->getHeight(chunkPos.add(j10, 0, i14)).getY() + 32;
		if (j17 > 0) 
		{
			auto k19 = random.nextInt(j17);
			auto blockpos6 = chunkPos.add(j10, k19, i14);
			auto blockflower$enumflowertype = biomeIn->pickRandomFlower(random, blockpos6);
			auto blockflower = blockflower$enumflowertype.getBlockType().getBlock();
			if (blockflower.getDefaultState().getMaterial() != Material::AIR) 
			{
				flowerGen.setGeneratedBlock(blockflower, blockflower$enumflowertype);
				flowerGen.generate(worldIn, random, blockpos6);
			}
		}
	}

	for (auto l5 = 0; l5 < grassPerChunk; ++l5)
	{
		auto j10 = random(16) + 8;
		auto i14 = random(16) + 8;
		auto j17 = worldIn(chunkPos.add(j10, 0, i14)).getY() * 2;
		if (j17 > 0) 
		{
			auto k19 = random(j17);
			biomeIn->getRandomWorldGenForGrass(random).generate(worldIn, random, chunkPos.add(j10, k19, i14));
		}
	}

	for (auto l5 = 0; l5 < deadBushPerChunk; ++l5)
	{
		auto j10 = random(16) + 8;
		auto i14 = random(16) + 8;
		auto j17 = worldIn->getHeight(chunkPos.add(j10, 0, i14)).getY() * 2;
		if (j17 > 0) 
		{
			auto k19 = random(j17);
			(new WorldGenDeadBush()).generate(worldIn, random, chunkPos.add(j10, k19, i14));
		}
	}

	for (auto l5 = 0; l5 < waterlilyPerChunk; ++l5)
	{
		auto j10 = random(16) + 8;
		auto i14 = random(16) + 8;
		auto j17 = worldIn->getHeight(chunkPos.add(j10, 0, i14)).getY() * 2;
		if (j17 > 0) 
		{
			auto k19 = random(j17);

			BlockPos blockpos7;
			auto blockpos6 = chunkPos.add(j10, k19, i14);
			for (; blockpos6.getY() > 0; blockpos6 = blockpos7) 
			{
				blockpos7 = blockpos6.down();
				if (!worldIn->isAirBlock(blockpos7)) 
				{
					break;
				}
			}

			waterlilyGen.generate(worldIn, random, blockpos6);
		}
	}

	for (auto l5 = 0; l5 < mushroomsPerChunk; ++l5)
	{
		if (random(4) == 0) 
		{
			auto j10 = random(16) + 8;
			auto i14 = random(16) + 8;
			BlockPos blockpos2 = worldIn->getHeight(chunkPos.add(j10, 0, i14));
			mushroomBrownGen.generate(worldIn, random, blockpos2);
		}

		if (random(8) == 0) 
		{
			auto j10 = random(16) + 8;
			auto i14 = random(16) + 8;
			auto j17 = worldIn->getHeight(chunkPos.add(j10, 0, i14)).getY() * 2;
			if (j17 > 0) 
			{
				auto k19 = random(j17);
				auto blockpos6 = chunkPos.add(j10, k19, i14);
				mushroomRedGen.generate(worldIn, random, blockpos6);
			}
		}
	}

	if (random(4) == 0) 
	{
		auto l5 = random(16) + 8;
		auto j10 = random(16) + 8;
		auto i14 = worldIn->getHeight(chunkPos.add(l5, 0, j10)).getY() * 2;
		if (i14 > 0) 
		{
			auto j17 = random(i14);
			mushroomBrownGen.generate(worldIn, random, chunkPos.add(l5, j17, j10));
		}
	}

	if (random(8) == 0) 
	{
		auto l5 = random(16) + 8;
		auto j10 = random(16) + 8;
		auto i14 = worldIn->getHeight(chunkPos.add(l5, 0, j10)).getY() * 2;
		if (i14 > 0) 
		{
			auto j17 = random(i14);
			mushroomRedGen.generate(worldIn, random, chunkPos.add(l5, j17, j10));
		}
	}

	for (auto l5 = 0; l5 < reedsPerChunk; ++l5) 
	{
		auto j10 = random(16) + 8;
		auto i14 = random(16) + 8;
		auto j17 = worldIn->getHeight(chunkPos.add(j10, 0, i14)).getY() * 2;
		if (j17 > 0) 
		{
			auto k19 = random(j17);
			reedGen.generate(worldIn, random, chunkPos.add(j10, k19, i14));
		}
	}

	for (auto l5 = 0; l5 < 10; ++l5)
	{
		auto j10 = random(16) + 8;
		auto i14 = random(16) + 8;
		auto j17 = worldIn->getHeight(chunkPos.add(j10, 0, i14)).getY() * 2;
		if (j17 > 0) 
		{
			auto k19 = random(j17);
			reedGen.generate(worldIn, random, chunkPos.add(j10, k19, i14));
		}
	}

	if (random(32) == 0) 
	{
		auto l5 = random(16) + 8;
		auto j10 = random(16) + 8;
		auto i14 = worldIn->getHeight(chunkPos.add(l5, 0, j10)).getY() * 2;
		if (i14 > 0) 
		{
			auto j17 = random(i14);
			(new WorldGenPumpkin()).generate(worldIn, random, chunkPos.add(l5, j17, j10));
		}
	}

	for (auto l5 = 0; l5 < cactiPerChunk; ++l5)
	{
		auto j10 = random(16) + 8;
		auto i14 = random(16) + 8;
		auto j17 = worldIn->getHeight(chunkPos.add(j10, 0, i14)).getY() * 2;
		if (j17 > 0) 
		{
			auto k19 = random(j17);
			cactusGen.generate(worldIn, random, chunkPos.add(j10, k19, i14));
		}
	}

	if (generateFalls) 
	{
		for (auto l5 = 0; l5 < 50; ++l5) 
		{
			auto j10 = random(16) + 8;
			auto i14 = random(16) + 8;
			auto j17 = random(248) + 8;
			if (j17 > 0) 
			{
				auto k19 = random(j17);
				auto blockpos6 = chunkPos.add(j10, k19, i14);
				(new WorldGenLiquids(Blocks.FLOWING_WATER)).generate(worldIn, random, blockpos6);
			}
		}

		for (auto l5 = 0; l5 < 20; ++l5)
		{
			auto j10 = random(16) + 8;
			auto i14 = random(16) + 8;
			auto j17 = random(random(random(240) + 8) + 8);
			auto blockpos3 = chunkPos.add(j10, j17, i14);
			(new WorldGenLiquids(Blocks.FLOWING_LAVA)).generate(worldIn, random, blockpos3);
		}
	}

}

void BiomeDecorator::generateOres(World* worldIn, pcg32& random)
{
	genStandardOre1(worldIn, random, chunkProviderSettings.dirtCount, dirtGen, chunkProviderSettings.dirtMinHeight, chunkProviderSettings.dirtMaxHeight);
	genStandardOre1(worldIn, random, chunkProviderSettings.gravelCount, gravelOreGen, chunkProviderSettings.gravelMinHeight, chunkProviderSettings.gravelMaxHeight);
	genStandardOre1(worldIn, random, chunkProviderSettings.dioriteCount, dioriteGen, chunkProviderSettings.dioriteMinHeight, chunkProviderSettings.dioriteMaxHeight);
	genStandardOre1(worldIn, random, chunkProviderSettings.graniteCount, graniteGen, chunkProviderSettings.graniteMinHeight, chunkProviderSettings.graniteMaxHeight);
	genStandardOre1(worldIn, random, chunkProviderSettings.andesiteCount, andesiteGen, chunkProviderSettings.andesiteMinHeight, chunkProviderSettings.andesiteMaxHeight);
	genStandardOre1(worldIn, random, chunkProviderSettings.coalCount, coalGen, chunkProviderSettings.coalMinHeight, chunkProviderSettings.coalMaxHeight);
	genStandardOre1(worldIn, random, chunkProviderSettings.ironCount, ironGen, chunkProviderSettings.ironMinHeight, chunkProviderSettings.ironMaxHeight);
	genStandardOre1(worldIn, random, chunkProviderSettings.goldCount, goldGen, chunkProviderSettings.goldMinHeight, chunkProviderSettings.goldMaxHeight);
	genStandardOre1(worldIn, random, chunkProviderSettings.redstoneCount, redstoneGen, chunkProviderSettings.redstoneMinHeight, chunkProviderSettings.redstoneMaxHeight);
	genStandardOre1(worldIn, random, chunkProviderSettings.diamondCount, diamondGen, chunkProviderSettings.diamondMinHeight, chunkProviderSettings.diamondMaxHeight);
	genStandardOre2(worldIn, random, chunkProviderSettings.lapisCount, lapisGen, chunkProviderSettings.lapisCenterHeight, chunkProviderSettings.lapisSpread);
}

void BiomeDecorator::genStandardOre1(World* worldIn, pcg32& random, int32_t blockCount, WorldGenerator generator, int32_t minHeight, int32_t maxHeight)
{
	if (maxHeight < minHeight) 
	{
		auto j = minHeight;
		minHeight = maxHeight;
		maxHeight = j;
	}
	else if (maxHeight == minHeight) 
	{
		if (minHeight < 255) 
		{
			++maxHeight;
		}
		else 
		{
			--minHeight;
		}
	}

	for (auto j = 0; j < blockCount; ++j)
	{
		BlockPos blockpos = chunkPos.add((int32_t)random(16), random(maxHeight - minHeight) + minHeight, random(16));
		generator.generate(worldIn, random, blockpos);
	}
}

void BiomeDecorator::genStandardOre2(World* worldIn, pcg32& random, int32_t blockCount, WorldGenerator generator, int32_t centerHeight, int32_t spread)
{
	for (auto i = 0; i < blockCount; ++i) 
	{
		auto blockpos = chunkPos.add((int32_t)random(16), random(spread) + random(spread) + centerHeight - spread, random(16));
		generator.generate(worldIn, random, blockpos);
	}
}
