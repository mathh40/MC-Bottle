#pragma once
#include "Biome.h"

class BiomeSnow :public Biome
{
public:
	BiomeSnow(bool superIcyIn, BiomeProperties properties);
	float getSpawningChance() override;
	void decorate(World* worldIn, pcg32& rand, BlockPos& pos) override;
	WorldGenAbstractTree getRandomTreeFeature(pcg32& rand) override;
private:
	bool superIcy;
	WorldGenIceSpike iceSpike = new WorldGenIceSpike();
	WorldGenIcePath icePatch = new WorldGenIcePath(4);
};
