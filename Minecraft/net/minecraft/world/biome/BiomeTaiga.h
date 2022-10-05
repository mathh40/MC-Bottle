#pragma once
#include "Biome.h"

class BiomeTaiga : public Biome
{
  public:
    enum class Type
    {
        NORMAL,
        MEGA,
        MEGA_SPRUCE
    };

    BiomeTaiga(Type typeIn, BiomeProperties properties);
    WorldGenAbstractTree getRandomTreeFeature(pcg32 &rand) override;
    WorldGenerator getRandomWorldGenForGrass(pcg32 &rand) override;
    void decorate(World *worldIn, pcg32 &rand, BlockPos &pos) override;
    void genTerrainBlocks(World *worldIn, pcg32 &rand, ChunkPrimer chunkPrimerIn, int32_t x, int32_t z,
                          double noiseVal) override;

  private:
    static WorldGenTaiga1 PINE_GENERATOR              = new WorldGenTaiga1();
    static WorldGenTaiga2 SPRUCE_GENERATOR            = new WorldGenTaiga2(false);
    static WorldGenMegaPineTree MEGA_PINE_GENERATOR   = new WorldGenMegaPineTree(false, false);
    static WorldGenMegaPineTree MEGA_SPRUCE_GENERATOR = new WorldGenMegaPineTree(false, true);
    static WorldGenBlockBlob FOREST_ROCK_GENERATOR;
    Type type;
};
