#pragma once
#include "BiomeSavanna.h"

class BiomeSavannaMutated : public BiomeSavanna
{
  public:
    BiomeSavannaMutated(BiomeProperties properties);
    void genTerrainBlocks(World *worldIn, pcg32 &rand, ChunkPrimer chunkPrimerIn, int32_t x, int32_t z,
                          double noiseVal) override;
    void decorate(World *worldIn, pcg32 &rand, BlockPos &pos) override;
};
