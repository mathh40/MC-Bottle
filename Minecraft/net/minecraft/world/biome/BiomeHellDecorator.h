#pragma once
#include "BiomeDecorator.h"

class BiomeHellDecorator : public BiomeDecorator
{
  public:
    void decorate(World *worldIn, pcg32 &random, Biome *biome, BlockPos &pos) override;
};
