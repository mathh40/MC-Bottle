#pragma once
#include "WorldGenerator.h"

class Block;

class WorldGenAbstractTree : public WorldGenerator
{
  public:
    WorldGenAbstractTree(bool notify);
    virtual void generateSaplings(World *worldIn, pcg32 &random, const BlockPos &pos);

  protected:
    bool canGrowInto(Block *blockType);
    void setDirtAt(World *worldIn, const BlockPos &pos);

  private:
};
