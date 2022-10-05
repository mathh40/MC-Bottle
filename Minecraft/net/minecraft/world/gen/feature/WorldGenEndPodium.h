#pragma once
#include "WorldGenerator.h"

class WorldGenEndPodium : public WorldGenerator
{
  public:
    static BlockPos END_PODIUM_LOCATION;
    static BlockPos END_PODIUM_CHUNK_POS;

    WorldGenEndPodium(bool activePortalIn);
    bool generate(World *worldIn, pcg32 &rand, const BlockPos &position) override;

  private:
    bool activePortal;
};
