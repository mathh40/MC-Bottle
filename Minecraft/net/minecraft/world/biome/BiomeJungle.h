#pragma once
#include "Biome.h"

class BiomeJungle : public Biome
{
  public:
    BiomeJungle(bool isEdgeIn, BiomeProperties properties);
    WorldGenAbstractTree getRandomTreeFeature(pcg32 &rand) override;
    WorldGenerator getRandomWorldGenForGrass(pcg32 &rand) override;
    void decorate(World *worldIn, pcg32 &rand, BlockPos &pos) override;

  private:
    bool isEdge;
    static IBlockState *JUNGLE_LOG;
    static IBlockState *JUNGLE_LEAF;
    static IBlockState *OAK_LEAF;
};
