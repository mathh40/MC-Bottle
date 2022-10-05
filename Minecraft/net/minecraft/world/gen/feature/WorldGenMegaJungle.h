#pragma once
#include "WorldGenHugeTrees.h"

class PropertyBool;

class WorldGenMegaJungle : public WorldGenHugeTrees
{
  public:
    WorldGenMegaJungle(bool notify, int32_t baseHeightIn, int32_t extraRandomHeightIn, IBlockState *woodMetadataIn,
                       IBlockState *p_i46448_5_);
    bool generate(World *worldIn, pcg32 &rand, const BlockPos &position) override;

  private:
    void placeVine(World *p_181632_1_, pcg32 &p_181632_2_, const BlockPos &p_181632_3_, PropertyBool *p_181632_4_);
    void createCrown(World *worldIn, const BlockPos &p_175930_2_, int32_t p_175930_3_);
};
