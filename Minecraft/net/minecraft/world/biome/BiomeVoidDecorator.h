#pragma once
#include "BiomeDecorator.h"

class BiomeVoidDecorator :public BiomeDecorator
{
	void decorate(World* worldIn, pcg32& random, Biome* biome, BlockPos& pos) override;
};
