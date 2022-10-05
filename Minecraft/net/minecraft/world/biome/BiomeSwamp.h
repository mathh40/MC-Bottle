#pragma once
#include "Biome.h"

class BiomeSwamp : public Biome
{
  public:
    WorldGenAbstractTree getRandomTreeFeature(pcg32 &rand) override;
    int32_t getGrassColorAtPos(BlockPos &pos) override;
    int32_t getFoliageColorAtPos(BlockPos &pos) override;
    BlockFlower.EnumFlowerType pickRandomFlower(pcg32 &rand, BlockPos &pos) override;
    void genTerrainBlocks(World *worldIn, pcg32 &rand, ChunkPrimer chunkPrimerIn, int32_t x, int32_t z,
                          double noiseVal) override;
    void decorate(World *worldIn, pcg32 &rand, BlockPos &pos) override;

  protected:
    static IBlockState *WATER_LILY;
    BiomeSwamp(BiomeProperties properties);

  private:
};
