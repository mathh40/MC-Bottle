#include "WorldGenShrub.h"

#include "World.h"

WorldGenShrub::WorldGenShrub(IBlockState *woodMetadataIn, IBlockState *leavesMetadataIn)
    : WorldGenTrees(false), woodMetadata(woodMetadataIn), leavesMetadata(leavesMetadataIn)
{
}

bool WorldGenShrub::generate(World *worldIn, pcg32 &rand, const BlockPos &positionin)
{
    BlockPos position = positionin;
    for (auto iblockstate = worldIn->getBlockState(position);
         (iblockstate->getMaterial() == Material::AIR || iblockstate->getMaterial() == Material::LEAVES) &&
         position.gety() > 0;
         iblockstate = worldIn->getBlockState(position))
    {
        position = position.down();
    }

    auto block = worldIn->getBlockState(position)->getBlock();
    if (block == Blocks::DIRT || block == Blocks::GRASS)
    {
        position = position.up();
        setBlockAndNotifyAdequately(worldIn, position, woodMetadata);

        for (auto i = position.gety(); i <= position.gety() + 2; ++i)
        {
            auto j = i - position.gety();
            auto k = 2 - j;

            for (auto l = position.getx() - k; l <= position.getx() + k; ++l)
            {
                auto i1 = l - position.getx();

                for (auto j1 = position.getz() - k; j1 <= position.getz() + k; ++j1)
                {
                    auto k1 = j1 - position.getz();
                    if (MathHelper::abs(i1) != k || MathHelper::abs(k1) != k || rand(2) != 0)
                    {
                        BlockPos blockpos(l, i, j1);
                        auto material = worldIn->getBlockState(blockpos)->getMaterial();
                        if (material == Material::AIR || material == Material::LEAVES)
                        {
                            setBlockAndNotifyAdequately(worldIn, blockpos, leavesMetadata);
                        }
                    }
                }
            }
        }
    }

    return true;
}
