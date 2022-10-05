#pragma once
#include "MapGenBase.h"

class MapGenCavesHell : public MapGenBase
{
  public:
  protected:
    static IBlockState *AIR;
    void addRoom(int64_t p_180703_1_, int32_t p_180703_3_, int32_t p_180703_4_, ChunkPrimer &p_180703_5_,
                 double p_180703_6_, double p_180703_8_, double p_180703_10_);
    void addTunnel(int64_t p_180702_1_, int32_t p_180702_3_, int32_t p_180702_4_, ChunkPrimer &p_180702_5_,
                   double p_180702_6_, double p_180702_8_, double p_180702_10_, float p_180702_12_, float p_180702_13_,
                   float p_180702_14_, int32_t p_180702_15_, int32_t p_180702_16_, double p_180702_17_);
    bool canReplaceBlock(IBlockState *p_175793_1_, IBlockState *p_175793_2_);
    void recursiveGenerate(World *worldIn, int32_t chunkX, int32_t chunkZ, int32_t originalX, int32_t originalZ,
                           ChunkPrimer &chunkPrimerIn) override;

  private:
};
