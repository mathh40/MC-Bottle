#pragma once
#include "Biome.h"

class BiomePlains :public Biome
{
public:
	BlockFlower.EnumFlowerType pickRandomFlower(pcg32& rand, BlockPos& pos) override;
	void decorate(World* worldIn, pcg32& rand, BlockPos& pos) override;
	WorldGenAbstractTree getRandomTreeFeature(pcg32& rand) override;
protected:
	bool sunflowers;
	BiomePlains(bool p_i46699_1_, BiomeProperties properties);
private:
};
