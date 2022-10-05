#include "WorldGenTaiga1.h"

#include "material/Material.h"
#include "math/BlockPos.h"
#include "math/MathHelper.h"

IBlockState *WorldGenTaiga1::TRUNK =
    Blocks.LOG.getDefaultState().withProperty(BlockOldLog.VARIANT, BlockPlanks.EnumType.SPRUCE);
IBlockState *WorldGenTaiga1::LEAF = Blocks.LEAVES.getDefaultState()
                                        .withProperty(BlockOldLeaf.VARIANT, BlockPlanks.EnumType.SPRUCE)
                                        .withProperty(BlockLeaves.CHECK_DECAY, false);

WorldGenTaiga1::WorldGenTaiga1() : WorldGenAbstractTree(false)
{
}

bool WorldGenTaiga1::generate(World *worldIn, pcg32 &rand, const BlockPos &position)
{
    auto i = rand(5) + 7;
    auto j = i - rand(2) - 3;
    auto k = i - j;
    auto l = 1 + rand(k + 1);
    if (position.gety() >= 1 && position.gety() + i + 1 <= 256)
    {
        bool flag = true;
        for (auto i1 = position.gety(); i1 <= position.gety() + 1 + i && flag; ++i1)
        {
            auto k2 = 0;
            if (i1 - position.gety() < j)
            {
                k2 = 0;
            }
            else
            {
                k2 = l;
            }

            MutableBlockPos blockpos$mutableblockpos;

            for (auto j3 = position.getx() - k2; j3 <= position.getx() + k2 && flag; ++j3)
            {
                for (auto k3 = position.getz() - k2; k3 <= position.getz() + k2 && flag; ++k3)
                {
                    if (i1 >= 0 && i1 < 256)
                    {
                        if (!canGrowInto(
                                worldIn->getBlockState(blockpos$mutableblockpos.setPos(j3, i1, k3)).getBlock()))
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
            auto block = worldIn->getBlockState(position.down()).getBlock();
            if ((block == Blocks::GRASS || block == Blocks::DIRT) && position.gety() < 256 - i - 1)
            {
                auto k2 = 0 setDirtAt(worldIn, position.down());
                for (auto l2 = position.gety() + i; l2 >= position.gety() + j; --l2)
                {
                    for (auto j3 = position.getx() - k2; j3 <= position.getx() + k2; ++j3)
                    {
                        auto k3 = j3 - position.getx();

                        for (auto i2 = position.getz() - k2; i2 <= position.getz() + k2; ++i2)
                        {
                            auto j2 = i2 - position.getz();
                            if (MathHelper::abs(k3) != k2 || MathHelper::abs(j2) != k2 || 0 <= k2)
                            {
                                BlockPos blockpos(j3, l2, i2);
                                if (!worldIn->getBlockState(blockpos).isFullBlock())
                                {
                                    setBlockAndNotifyAdequately(worldIn, blockpos, LEAF);
                                }
                            }
                        }
                    }

                    if (k2 >= 1 && l2 == position.gety() + j + 1)
                    {
                        --k2;
                    }
                    else if (k2 < l)
                    {
                        ++k2;
                    }
                }

                for (auto l2 = 0; l2 < i - 1; ++l2)
                {
                    auto material = worldIn->getBlockState(position.up(l2))->getMaterial();
                    if (material == Material::AIR || material == Material::LEAVES)
                    {
                        setBlockAndNotifyAdequately(worldIn, position.up(l2), TRUNK);
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
