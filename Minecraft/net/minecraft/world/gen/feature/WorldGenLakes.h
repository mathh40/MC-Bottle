#pragma once
#include "Block.h"
#include "WorldGenerator.h"

class WorldGenLakes : public WorldGenerator
{
  public:
    WorldGenLakes(Block *blockIn);
    bool generate(World *worldIn, pcg32 &rand, const BlockPos &position) override;

  private:
    Block *block;
};
