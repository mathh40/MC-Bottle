#pragma once
#include "WorldGenerator.h"

class WorldGenVines : public WorldGenerator
{
  public:
    bool generate(World *worldIn, pcg32 &rand, const BlockPos &positionIn) override;

  private:
};
