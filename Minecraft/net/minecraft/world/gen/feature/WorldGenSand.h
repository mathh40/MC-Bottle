#pragma once
#include "Block.h"
#include "WorldGenerator.h"

class WorldGenSand : public WorldGenerator
{
  public:
    WorldGenSand(Block *blockIn, int32_t radiusIn);
    bool generate(World *worldIn, pcg32 &rand, const BlockPos &position) override;

  private:
    Block *block;
    int32_t radius;
};
