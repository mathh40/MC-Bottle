#pragma once
#include "Biome.h"

class BiomeDesert : public Biome
{
  public:
    BiomeDesert(BiomeProperties properties);
    void decorate(World *worldIn, pcg32 &rand, BlockPos &pos) override;

  private:
};
