#include "WorldGenHugeTrees.h"

#include "World.h"
#include "math/BlockPos.h"

WorldGenHugeTrees::WorldGenHugeTrees(bool notify, int32_t baseHeightIn, int32_t extraRandomHeightIn,
                                     IBlockState *woodMetadataIn, IBlockState *leavesMetadataIn)
    : WorldGenAbstractTree(notify), baseHeight(baseHeightIn), extraRandomHeight(extraRandomHeightIn),
      woodMetadata(woodMetadataIn), leavesMetadata(leavesMetadataIn)
{
}

int32_t WorldGenHugeTrees::getHeight(pcg32 &rand) const
{
    auto i = rand(3) + baseHeight;
    if (extraRandomHeight > 1)
    {
        i += rand(extraRandomHeight);
    }

    return i;
}

bool WorldGenHugeTrees::ensureGrowable(World *worldIn, pcg32 &rand, const BlockPos &treePos, int32_t height)
{
    return isSpaceAt(worldIn, treePos, height) && ensureDirtsUnderneath(treePos, worldIn);
}

void WorldGenHugeTrees::growLeavesLayerStrict(World *worldIn, const BlockPos &layerCenter, int32_t width)
{
    auto i = width * width;

    for (auto j = -width; j <= width + 1; ++j)
    {
        for (auto k = -width; k <= width + 1; ++k)
        {
            auto l  = j - 1;
            auto i1 = k - 1;
            if (j * j + k * k <= i || l * l + i1 * i1 <= i || j * j + i1 * i1 <= i || l * l + k * k <= i)
            {
                auto blockpos = layerCenter.add(j, 0, k);
                auto material = worldIn->getBlockState(blockpos)->getMaterial();
                if (material == Material::AIR || material == Material::LEAVES)
                {
                    setBlockAndNotifyAdequately(worldIn, blockpos, leavesMetadata);
                }
            }
        }
    }
}

void WorldGenHugeTrees::growLeavesLayer(World *worldIn, const BlockPos &layerCenter, int32_t width)
{
    auto i = width * width;

    for (auto j = -width; j <= width; ++j)
    {
        for (auto k = -width; k <= width; ++k)
        {
            if (j * j + k * k <= i)
            {
                auto blockpos = layerCenter.add(j, 0, k);
                auto material = worldIn->getBlockState(blockpos)->getMaterial();
                if (material == Material::AIR || material == Material::LEAVES)
                {
                    setBlockAndNotifyAdequately(worldIn, blockpos, leavesMetadata);
                }
            }
        }
    }
}

bool WorldGenHugeTrees::isSpaceAt(World *worldIn, const BlockPos &leavesPos, int32_t height)
{
    bool flag = true;
    if (leavesPos.gety() >= 1 && leavesPos.gety() + height + 1 <= 256)
    {
        for (auto i = 0; i <= 1 + height; ++i)
        {
            auto j = 2;
            if (i == 0)
            {
                j = 1;
            }
            else if (i >= 1 + height - 2)
            {
                j = 2;
            }

            for (auto k = -j; k <= j && flag; ++k)
            {
                for (auto l = -j; l <= j && flag; ++l)
                {
                    if (leavesPos.gety() + i < 0 || leavesPos.gety() + i >= 256 ||
                        !canGrowInto(worldIn->getBlockState(leavesPos.add(k, i, l))->getBlock()))
                    {
                        flag = false;
                    }
                }
            }
        }

        return flag;
    }
    else
    {
        return false;
    }
}

bool WorldGenHugeTrees::ensureDirtsUnderneath(const BlockPos &pos, World *worldIn)
{
    auto blockpos = pos.down();
    Block *block  = worldIn->getBlockState(blockpos)->getBlock();
    if ((block == Blocks::GRASS || block == Blocks::DIRT) && pos.gety() >= 2)
    {
        setDirtAt(worldIn, blockpos);
        setDirtAt(worldIn, blockpos.east());
        setDirtAt(worldIn, blockpos.south());
        setDirtAt(worldIn, blockpos.south().east());
        return true;
    }
    else
    {
        return false;
    }
}
