#pragma once
#include "Biome.h"

class BiomeSavanna :public Biome
{
public:
	WorldGenAbstractTree getRandomTreeFeature(pcg32& rand) override;
	void decorate(World* worldIn, pcg32& rand, BlockPos& pos) override;
protected:
	BiomeSavanna(BiomeProperties properties);
private:
	static WorldGenSavannaTree SAVANNA_TREE = new WorldGenSavannaTree(false);
};
