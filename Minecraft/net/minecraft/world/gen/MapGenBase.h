#pragma once
#include "World.h"

class MapGenBase
{
  public:
    virtual ~MapGenBase() = default;
    void generate(World *worldIn, int32_t x, int32_t z, ChunkPrimer &primer);
    void setupChunkSeed(int64_t p_191068_0_, pcg32 &p_191068_2_, int64_t p_191068_3_, int64_t p_191068_4_);

  protected:
    int32_t range = 8;
    pcg32 rand;
    World *world;

    virtual void recursiveGenerate(World *worldIn, int32_t chunkX, int32_t chunkZ, int32_t originalX, int32_t originalZ,
                                   ChunkPrimer &chunkPrimerIn);

  private:
};
