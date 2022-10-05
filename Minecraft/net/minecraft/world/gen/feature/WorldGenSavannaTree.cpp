#include "WorldGenSavannaTree.h"

#include "World.h"
#include "math/BlockPos.h"

IBlockState *WorldGenSavannaTree::TRUNK =
    Blocks::LOG2.getDefaultState().withProperty(BlockNewLog.VARIANT, BlockPlanks.EnumType.ACACIA);
IBlockState *WorldGenSavannaTree::LEAF = Blocks::LEAVES2.getDefaultState()
                                             .withProperty(BlockNewLeaf.VARIANT, BlockPlanks.EnumType.ACACIA)
                                             .withProperty(BlockLeaves.CHECK_DECAY, false);

WorldGenSavannaTree::WorldGenSavannaTree(bool doBlockNotify) : WorldGenAbstractTree(doBlockNotify)
{
}

bool WorldGenSavannaTree::generate(World *worldIn, pcg32 &rand, const BlockPos &position)
{
    auto i    = rand(3) + rand(3) + 5;
    auto flag = true;
    if (position.gety() >= 1 && position.gety() + i + 1 <= 256)
    {
        for (auto j = position.gety(); j <= position.gety() + 1 + i; ++j)
        {
            auto k = 1;
            if (j == position.gety())
            {
                k = 0;
            }

            if (j >= position.gety() + 1 + i - 2)
            {
                k = 2;
            }

            MutableBlockPos blockpos$mutableblockpos;

            for (auto l = position.getx() - k; l <= position.getx() + k && flag; ++l)
            {
                for (auto i3 = position.getz() - k; i3 <= position.getz() + k && flag; ++i3)
                {
                    if (j >= 0 && j < 256)
                    {
                        if (!canGrowInto(worldIn->getBlockState(blockpos$mutableblockpos.setPos(l, j, i3)).getBlock()))
                        {
                            flag = false;
                        }
                    }
                    else
                    {
                        flag = false;
                    }
                }
            }
        }

        if (!flag)
        {
            return false;
        }
        else
        {
            auto block = worldIn->getBlockState(position.down())->getBlock();
            if ((block == Blocks::GRASS || block == Blocks::DIRT) && position.gety() < 256 - i - 1)
            {
                setDirtAt(worldIn, position.down());
                EnumFacing enumfacing = Plane::HORIZONTAL.random(rand);
                auto k2               = i - rand(4) - 1;
                auto l                = 3 - rand(3);
                auto i3               = position.getx();
                auto j1               = position.getz();
                auto k1               = 0;

                for (auto l1 = 0; l1 < i; ++l1)
                {
                    auto k3 = position.gety() + l1;
                    if (l1 >= k2 && l > 0)
                    {
                        i3 += enumfacing.getXOffset();
                        j1 += enumfacing.getZOffset();
                        --l;
                    }

                    BlockPos blockpos(i3, k3, j1);
                    auto material = worldIn->getBlockState(blockpos)->getMaterial();
                    if (material == Material::AIR || material == Material::LEAVES)
                    {
                        placeLogAt(worldIn, blockpos);
                        k1 = k3;
                    }
                }

                BlockPos blockpos2(i3, k1, j1);

                for (auto k3 = -3; k3 <= 3; ++k3)
                {
                    for (auto l3 = -3; l3 <= 3; ++l3)
                    {
                        if (MathHelper::abs(k3) != 3 || MathHelper::abs(l3) != 3)
                        {
                            placeLeafAt(worldIn, blockpos2.add(k3, 0, l3));
                        }
                    }
                }

                blockpos2 = blockpos2.up();

                for (auto k3 = -1; k3 <= 1; ++k3)
                {
                    for (auto l3 = -1; l3 <= 1; ++l3)
                    {
                        placeLeafAt(worldIn, blockpos2.add(k3, 0, l3));
                    }
                }

                placeLeafAt(worldIn, blockpos2.east(2));
                placeLeafAt(worldIn, blockpos2.west(2));
                placeLeafAt(worldIn, blockpos2.south(2));
                placeLeafAt(worldIn, blockpos2.north(2));
                i3                     = position.getx();
                j1                     = position.getz();
                EnumFacing enumfacing1 = Plane::HORIZONTAL.random(rand);
                if (enumfacing1 != enumfacing)
                {
                    auto l3 = k2 - rand(2) - 1;
                    auto k4 = 1 + rand(3);
                    k1      = 0;

                    for (auto l4 = l3; l4 < i && k4 > 0; --k4)
                    {
                        if (l4 >= 1)
                        {
                            auto j5 = position.gety() + l4;
                            i3 += enumfacing1.getXOffset();
                            j1 += enumfacing1.getZOffset();
                            BlockPos blockpos1(i3, j5, j1);
                            Material material1 = worldIn->getBlockState(blockpos1)->getMaterial();
                            if (material1 == Material::AIR || material1 == Material::LEAVES)
                            {
                                placeLogAt(worldIn, blockpos1);
                                k1 = j5;
                            }
                        }

                        ++l4;
                    }

                    if (k1 > 0)
                    {
                        BlockPos blockpos3(i3, k1, j1);
                        for (auto j5 = -2; j5 <= 2; ++j5)
                        {
                            for (auto l5 = -2; l5 <= 2; ++l5)
                            {
                                if (MathHelper::abs(j5) != 2 || MathHelper::abs(l5) != 2)
                                {
                                    placeLeafAt(worldIn, blockpos3.add(j5, 0, l5));
                                }
                            }
                        }

                        blockpos3 = blockpos3.up();

                        for (auto j5 = -1; j5 <= 1; ++j5)
                        {
                            for (auto l5 = -1; l5 <= 1; ++l5)
                            {
                                placeLeafAt(worldIn, blockpos3.add(j5, 0, l5));
                            }
                        }
                    }
                }

                return true;
            }
            else
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }
}

void WorldGenSavannaTree::placeLogAt(World *worldIn, const BlockPos &pos)
{
    setBlockAndNotifyAdequately(worldIn, pos, TRUNK);
}

void WorldGenSavannaTree::placeLeafAt(World *worldIn, const BlockPos &pos)
{
    auto material = worldIn->getBlockState(pos)->getMaterial();
    if (material == Material::AIR || material == Material::LEAVES)
    {
        setBlockAndNotifyAdequately(worldIn, pos, LEAF);
    }
}
