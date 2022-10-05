#pragma once
#include "Block.h"
#include "WorldGenerator.h"

class WorldGenClay : public WorldGenerator
{
  public:
    WorldGenClay(int32_t p_i2011_1_);

    bool generate(World *worldIn, pcg32 &rand, const BlockPos &position) override;

  private:
    Block *block;
    int32_t numberOfBlocks;
};
