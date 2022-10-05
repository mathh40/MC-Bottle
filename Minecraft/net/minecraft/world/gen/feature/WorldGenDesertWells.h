#pragma once
#include "WorldGenerator.h"

class WorldGenDesertWells : public WorldGenerator
{
  public:
    WorldGenDesertWells();
    bool generate(World *worldIn, pcg32 &rand, const BlockPos &position) override;

  private:
    static BlockStateMatcher IS_SAND;
    IBlockState *sandSlab;
    IBlockState *sandstone;
    IBlockState *water;
};
