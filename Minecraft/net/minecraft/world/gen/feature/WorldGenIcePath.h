#pragma once
#include "Block.h"
#include "WorldGenerator.h"

class WorldGenIcePath : public WorldGenerator
{
  public:
    WorldGenIcePath(int32_t basePathWidthIn);
    bool generate(World *worldIn, pcg32 &rand, const BlockPos &position) override;

  private:
    Block *block;
    int32_t basePathWidth;
};
