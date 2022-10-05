#pragma once
#include "WorldGenAbstractTree.h"

class WorldGenHugeTrees : public WorldGenAbstractTree
{
  public:
    WorldGenHugeTrees(bool notify, int32_t baseHeightIn, int32_t extraRandomHeightIn, IBlockState *woodMetadataIn,
                      IBlockState *leavesMetadataIn);

  protected:
    int32_t baseHeight;
    IBlockState *woodMetadata;
    IBlockState *leavesMetadata;
    int32_t extraRandomHeight;

    int32_t getHeight(pcg32 &rand) const;
    bool ensureGrowable(World *worldIn, pcg32 &rand, const BlockPos &treePos, int32_t height);
    void growLeavesLayerStrict(World *worldIn, const BlockPos &layerCenter, int32_t width);
    void growLeavesLayer(World *worldIn, const BlockPos &layerCenter, int32_t width);

  private:
    bool isSpaceAt(World *worldIn, const BlockPos &leavesPos, int32_t height);
    bool ensureDirtsUnderneath(const BlockPos &pos, World *worldIn);
};
