#include "WorldGenCanopyTree.h"

#include "World.h"
#include "math/BlockPos.h"

IBlockState *WorldGenCanopyTree::DARK_OAK_LOG =
    Blocks.LOG2.getDefaultState().withProperty(BlockNewLog.VARIANT, BlockPlanks.EnumType.DARK_OAK);
IBlockState *WorldGenCanopyTree::DARK_OAK_LEAVES =
    Blocks.LEAVES2.getDefaultState()
        .withProperty(BlockNewLeaf.VARIANT, BlockPlanks.EnumType.DARK_OAK)
        .withProperty(BlockLeaves.CHECK_DECAY, false);

WorldGenCanopyTree::WorldGenCanopyTree(bool notify) : WorldGenAbstractTree(notify)
{
}

bool WorldGenCanopyTree::generate(World *worldIn, pcg32 &rand, const BlockPos &position)
{
    auto i = rand(3) + rand(2) + 6;
    auto j = position.getx();
    auto k = position.gety();
    auto l = position.getz();
    if (k >= 1 && k + i + 1 < 256)
    {
        BlockPos blockpos = position.down();
        auto block        = worldIn->getBlockState(blockpos).getBlock();
        if (block != Blocks::GRASS && block != Blocks::DIRT)
        {
            return false;
        }
        else if (!placeTreeOfHeight(worldIn, position, i))
        {
            return false;
        }
        else
        {
            setDirtAt(worldIn, blockpos);
            setDirtAt(worldIn, blockpos.east());
            setDirtAt(worldIn, blockpos.south());
            setDirtAt(worldIn, blockpos.south().east());
            EnumFacing enumfacing = EnumFacing::Plane::HORIZONTAL.random(rand);
            auto i1               = i - rand(4);
            auto j1               = 2 - rand(3);
            auto k1               = j;
            auto l1               = l;
            auto i2               = k + i - 1;

            for (auto k3 = 0; k3 < i; ++k3)
            {
                if (k3 >= i1 && j1 > 0)
                {
                    k1 += enumfacing.getXOffset();
                    l1 += enumfacing.getZOffset();
                    --j1;
                }

                auto j4 = k + k3;
                BlockPos blockpos1(k1, j4, l1);
                Material material = worldIn->getBlockState(blockpos1).getMaterial();
                if (material == Material::AIR || material == Material::LEAVES)
                {
                    placeLogAt(worldIn, blockpos1);
                    placeLogAt(worldIn, blockpos1.east());
                    placeLogAt(worldIn, blockpos1.south());
                    placeLogAt(worldIn, blockpos1.east().south());
                }
            }

            for (auto k3 = -2; k3 <= 0; ++k3)
            {
                for (auto j4 = -2; j4 <= 0; ++j4)
                {
                    auto k4 = -1;
                    placeLeafAt(worldIn, k1 + k3, i2 + k4, l1 + j4);
                    placeLeafAt(worldIn, 1 + k1 - k3, i2 + k4, l1 + j4);
                    placeLeafAt(worldIn, k1 + k3, i2 + k4, 1 + l1 - j4);
                    placeLeafAt(worldIn, 1 + k1 - k3, i2 + k4, 1 + l1 - j4);
                    if ((k3 > -2 || j4 > -1) && (k3 != -1 || j4 != -2))
                    {
                        auto k4 = 1;
                        placeLeafAt(worldIn, k1 + k3, i2 + k4, l1 + j4);
                        placeLeafAt(worldIn, 1 + k1 - k3, i2 + k4, l1 + j4);
                        placeLeafAt(worldIn, k1 + k3, i2 + k4, 1 + l1 - j4);
                        placeLeafAt(worldIn, 1 + k1 - k3, i2 + k4, 1 + l1 - j4);
                    }
                }
            }
            Util::RandomizerWithSentinelShift<> sh;
            if (sh(rand))
            {
                placeLeafAt(worldIn, k1, i2 + 2, l1);
                placeLeafAt(worldIn, k1 + 1, i2 + 2, l1);
                placeLeafAt(worldIn, k1 + 1, i2 + 2, l1 + 1);
                placeLeafAt(worldIn, k1, i2 + 2, l1 + 1);
            }

            for (auto k3 = -3; k3 <= 4; ++k3)
            {
                for (auto j4 = -3; j4 <= 4; ++j4)
                {
                    if ((k3 != -3 || j4 != -3) && (k3 != -3 || j4 != 4) && (k3 != 4 || j4 != -3) &&
                        (k3 != 4 || j4 != 4) && (MathHelper::abs(k3) < 3 || MathHelper::abs(j4) < 3))
                    {
                        placeLeafAt(worldIn, k1 + k3, i2, l1 + j4);
                    }
                }
            }

            for (auto k3 = -1; k3 <= 2; ++k3)
            {
                for (auto j4 = -1; j4 <= 2; ++j4)
                {
                    if ((k3 < 0 || k3 > 1 || j4 < 0 || j4 > 1) && rand(3) <= 0)
                    {
                        auto l4 = rand(3) + 2;

                        for (auto k5 = 0; k5 < l4; ++k5)
                        {
                            placeLogAt(worldIn, BlockPos(j + k3, i2 - k5 - 1, l + j4));
                        }

                        for (auto k5 = -1; k5 <= 1; ++k5)
                        {
                            for (auto l5 = -1; l5 <= 1; ++l5)
                            {
                                placeLeafAt(worldIn, k1 + k3 + k5, i2, l1 + j4 + l5);
                            }
                        }

                        for (auto k5 = -2; k5 <= 2; ++k5)
                        {
                            for (auto l5 = -2; l5 <= 2; ++l5)
                            {
                                if (MathHelper::abs(k5) != 2 || MathHelper::abs(l5) != 2)
                                {
                                    placeLeafAt(worldIn, k1 + k3 + k5, i2 - 1, l1 + j4 + l5);
                                }
                            }
                        }
                    }
                }
            }

            return true;
        }
    }
    else
    {
        return false;
    }
}

bool WorldGenCanopyTree::placeTreeOfHeight(World *worldIn, const BlockPos &pos, int32_t height)
{
    auto i = pos.getx();
    auto j = pos.gety();
    auto k = pos.getz();
    MutableBlockPos blockpos$mutableblockpos;

    for (auto l = 0; l <= height + 1; ++l)
    {
        auto i1 = 1;
        if (l == 0)
        {
            i1 = 0;
        }

        if (l >= height - 1)
        {
            i1 = 2;
        }

        for (auto j1 = -i1; j1 <= i1; ++j1)
        {
            for (auto k1 = -i1; k1 <= i1; ++k1)
            {
                if (!canGrowInto(
                        worldIn->getBlockState(blockpos$mutableblockpos.setPos(i + j1, j + l, k + k1)).getBlock()))
                {
                    return false;
                }
            }
        }
    }

    return true;
}

void WorldGenCanopyTree::placeLogAt(World *worldIn, const BlockPos &pos)
{
    if (canGrowInto(worldIn->getBlockState(pos).getBlock()))
    {
        setBlockAndNotifyAdequately(worldIn, pos, DARK_OAK_LOG);
    }
}

void WorldGenCanopyTree::placeLeafAt(World *worldIn, int32_t x, int32_t y, int32_t z)
{
    BlockPos blockpos(x, y, z);
    Material material = worldIn->getBlockState(blockpos).getMaterial();
    if (material == Material::AIR)
    {
        setBlockAndNotifyAdequately(worldIn, blockpos, DARK_OAK_LEAVES);
    }
}
