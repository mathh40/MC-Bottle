#pragma once
#include "Biome.h"
#include "gen/feature/WorldGenAbstractTree.h"
#include "gen/NoiseGeneratorPerlin.h"

class ChunkPrimer;

class BiomeMesa :public Biome
{
public:

	class Decorator :public BiomeDecorator
	{
	protected:
		void generateOres(World* worldIn, pcg32& random) override;
	};

	BiomeMesa(bool p_i46704_1_, bool p_i46704_2_, BiomeProperties properties);

	WorldGenAbstractTree getRandomTreeFeature(pcg32& rand) override;
	int32_t getFoliageColorAtPos(BlockPos& pos) override;
	int32_t getGrassColorAtPos(BlockPos& pos) override;
	void genTerrainBlocks(World* worldIn, pcg32& rand, ChunkPrimer chunkPrimerIn, int32_t x, int32_t z, double noiseVal) override;
protected:
	static IBlockState* COARSE_DIRT;
	static IBlockState* GRASS;
	static IBlockState* HARDENED_CLAY;
	static IBlockState* STAINED_HARDENED_CLAY;
	static IBlockState* ORANGE_STAINED_HARDENED_CLAY;
	static IBlockState* RED_SAND;

	BiomeDecorator createBiomeDecorator() override;
private:
	std::array<IBlockState*, 64> clayBands;
	int64_t worldSeed;
	NoiseGeneratorPerlin pillarNoise;
	NoiseGeneratorPerlin pillarRoofNoise;
	NoiseGeneratorPerlin clayBandsOffsetNoise;
	bool brycePillars;
	bool hasForest;

	void generateBands(int64_t p_150619_1_);
	IBlockState* getBand(int32_t p_180629_1_, int32_t p_180629_2_, int32_t p_180629_3_);
};
