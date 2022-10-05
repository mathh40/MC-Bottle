#pragma once
#include "WorldGenerator.h"

class WorldGenGlowStone1 : public WorldGenerator
{
  public:
    bool generate(World *worldIn, pcg32 &rand, const BlockPos &position) override;
};
