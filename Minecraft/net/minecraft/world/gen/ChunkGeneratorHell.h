#pragma once
#include "IChunkGenerator.h"
#include "MapGenBase.h"
#include "NoiseGeneratorOctaves.h"

class ChunkPrimer;

class ChunkGeneratorHell : public IChunkGenerator
{
  public:
    NoiseGeneratorOctaves *scaleNoise;
    NoiseGeneratorOctaves *depthNoise;

    ChunkGeneratorHell(World *worldIn, bool p_i45637_2_, int64_t seed);
    void prepareHeights(int32_t p_185936_1_, int32_t p_185936_2_, ChunkPrimer &primer);
    void buildSurfaces(int32_t p_185937_1_, int32_t p_185937_2_, ChunkPrimer &primer);
    Chunk generateChunk(int32_t x, int32_t z) override;
    void populate(int32_t x, int32_t z) override;
    bool generateStructures(Chunk &chunkIn, int32_t x, int32_t z) override;
    std::vector<SpawnListEntry> getPossibleCreatures(EnumCreatureType creatureType, BlockPos &pos) override;
    std::optional<BlockPos> getNearestStructurePos(World *worldIn, std::string_view structureName, BlockPos &position,
                                                   bool findUnexplored) override;
    bool isInsideStructure(World *worldIn, std::string_view structureName, BlockPos &pos) override;
    void recreateStructures(Chunk &chunkIn, int32_t x, int32_t z) override;

  protected:
    static IBlockState *AIR;
    static IBlockState *NETHERRACK;
    static IBlockState *BEDROCK;
    static IBlockState *LAVA;
    static IBlockState *GRAVEL;
    static IBlockState *SOUL_SAND;

  private:
    World *world;
    bool bgenerateStructures;
    pcg32 rand;
    std::array<double, 256> slowsandNoise;
    std::array<double, 256> gravelNoise;
    std::array<double, 256> depthBuffer;
    std::vector<double> buffer;
    NoiseGeneratorOctaves *lperlinNoise1;
    NoiseGeneratorOctaves *lperlinNoise2;
    NoiseGeneratorOctaves *perlinNoise1;
    NoiseGeneratorOctaves *slowsandGravelNoiseGen;
    NoiseGeneratorOctaves *netherrackExculsivityNoiseGen;
    WorldGenFire fireFeature;
    WorldGenGlowStone1 lightGemGen;
    WorldGenGlowStone2 hellPortalGen;
    WorldGenerator quartzGen;
    WorldGenerator magmaGen;
    WorldGenHellLava lavaTrapGen;
    WorldGenHellLava hellSpringGen;
    WorldGenBush brownMushroomFeature;
    WorldGenBush redMushroomFeature;
    MapGenNetherBridge genNetherBridge;
    MapGenBase genNetherCaves;

    std::vector<double> pnr;
    std::vector<double> ar;
    std::vector<double> br;
    std::vector<double> noiseData4;
    std::vector<double> dr;

    std::vector<double> getHeights(std::vector<double> &p_185938_1_, int32_t p_185938_2_, int32_t p_185938_3_,
                                   int32_t p_185938_4_, int32_t p_185938_5_, int32_t p_185938_6_, int32_t p_185938_7_);
};
