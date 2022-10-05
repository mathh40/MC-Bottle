#pragma once
#include "WorldGenerator.h"

class WorldGenBush : public WorldGenerator
{
  public:
    WorldGenBush(BlockBush blockIn);
    bool generate(World *worldIn, pcg32 &rand, const BlockPos &position) override;

  private:
    BlockBush block;
};
