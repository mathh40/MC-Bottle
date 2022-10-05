#pragma once
#include "Block.h"
#include "WorldGenerator.h"

class WorldGenBigMushroom : public WorldGenerator
{
  public:
    WorldGenBigMushroom(Block *p_i46449_1_);
    WorldGenBigMushroom();
    bool generate(World *worldIn, pcg32 &rand, const BlockPos &position) override;

  private:
    Block *mushroomType;
};
