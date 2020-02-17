#pragma once
#include "Biome.h"

class BiomeHills :public Biome
{
public:
	enum class Type
	{
		NORMAL,
		EXTRA_TREES,
		MUTATED
	};

	WorldGenAbstractTree getRandomTreeFeature(pcg32& rand) override;
	void decorate(World* worldIn, pcg32& rand, BlockPos& pos) override;
	void genTerrainBlocks(World* worldIn, pcg32& rand, ChunkPrimer chunkPrimerIn, int32_t x, int32_t z, double noiseVal) override;
protected:
	BiomeHills(Type p_i46710_1_, BiomeProperties properties);
private: 
	WorldGenerator silverfishSpawner;
	WorldGenTaiga2 spruceGenerator;
	Type type;
};
