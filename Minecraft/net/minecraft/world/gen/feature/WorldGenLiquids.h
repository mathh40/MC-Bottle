#pragma once
#include "Block.h"
#include "WorldGenerator.h"

class WorldGenLiquids : public WorldGenerator
{
  public:
    WorldGenLiquids(Block *blockIn);
    bool generate(World *worldIn, pcg32 &rand, const BlockPos &position) override;

  private:
    Block *block;
};
