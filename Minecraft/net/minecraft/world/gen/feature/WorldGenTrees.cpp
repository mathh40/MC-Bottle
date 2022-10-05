#include "WorldGenTrees.h"

#include "World.h"
#include "math/BlockPos.h"
#include "properties/PropertyBool.h"

IBlockState *WorldGenTrees::DEFAULT_TRUNK =
    Blocks::LOG.getDefaultState().withProperty(BlockOldLog.VARIANT, BlockPlanks.EnumType.OAK);
IBlockState *WorldGenTrees::DEFAULT_LEAF = Blocks::LEAVES.getDefaultState()
                                               .withProperty(BlockOldLeaf.VARIANT, BlockPlanks.EnumType.OAK)
                                               .withProperty(BlockLeaves.CHECK_DECAY, false);

WorldGenTrees::WorldGenTrees(bool notify) : WorldGenTrees(notify, 4, DEFAULT_TRUNK, DEFAULT_LEAF, false)
{
}

WorldGenTrees::WorldGenTrees(bool notify, int32_t minTreeHeightIn, IBlockState *woodMeta, IBlockState *p_i46446_4_,
                             bool growVines)
    : WorldGenAbstractTree(notify), minTreeHeight(minTreeHeightIn), metaWood(woodMeta), metaLeaves(p_i46446_4_),
      vinesGrow(growVines)
{
}

bool WorldGenTrees::generate(World *worldIn, pcg32 &rand, const BlockPos &position)
{
    auto i    = rand(3) + minTreeHeight;
    bool flag = true;
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

            for (auto l3 = position.getx() - k; l3 <= position.getx() + k && flag; ++l3)
            {
                for (auto i4 = position.getz() - k; i4 <= position.getz() + k && flag; ++i4)
                {
                    if (j >= 0 && j < 256)
                    {
                        if (!canGrowInto(
                                worldIn->getBlockState(blockpos$mutableblockpos.setPos(l3, j, i4))->getBlock()))
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
            if ((block == Blocks::GRASS || block == Blocks::DIRT || block == Blocks::FARMLAND) &&
                position.gety() < 256 - i - 1)
            {
                setDirtAt(worldIn, position.down());

                BlockPos blockpos3;
                for (auto l3 = position.gety() - 3 + i; l3 <= position.gety() + i; ++l3)
                {
                    auto i4 = l3 - (position.gety() + i);
                    auto k4 = 1 - i4 / 2;

                    for (auto k1 = position.getx() - k4; k1 <= position.getx() + k4; ++k1)
                    {
                        auto l4 = k1 - position.getx();

                        for (auto i5 = position.getz() - k4; i5 <= position.getz() + k4; ++i5)
                        {
                            auto j2 = i5 - position.getz();
                            if (MathHelper::abs(l4) != k4 || MathHelper::abs(j2) != k4 || rand(2) != 0 && i4 != 0)
                            {
                                blockpos3     = BlockPos(k1, l3, i5);
                                auto material = worldIn->getBlockState(blockpos3)->getMaterial();
                                if (material == Material::AIR || material == Material::LEAVES ||
                                    material == Material::VINE)
                                {
                                    setBlockAndNotifyAdequately(worldIn, blockpos3, metaLeaves);
                                }
                            }
                        }
                    }
                }

                for (auto l3 = 0; l3 < i; ++l3)
                {
                    auto material1 = worldIn->getBlockState(position.up(l3))->getMaterial();
                    if (material1 == Material::AIR || material1 == Material::LEAVES || material1 == Material::VINE)
                    {
                        setBlockAndNotifyAdequately(worldIn, position.up(l3), metaWood);
                        if (vinesGrow && l3 > 0)
                        {
                            if (rand(3) > 0 && worldIn->isAirBlock(position.add(-1, l3, 0)))
                            {
                                addVine(worldIn, position.add(-1, l3, 0), BlockVine::EAST);
                            }

                            if (rand(3) > 0 && worldIn->isAirBlock(position.add(1, l3, 0)))
                            {
                                addVine(worldIn, position.add(1, l3, 0), BlockVine::WEST);
                            }

                            if (rand(3) > 0 && worldIn->isAirBlock(position.add(0, l3, -1)))
                            {
                                addVine(worldIn, position.add(0, l3, -1), BlockVine::SOUTH);
                            }

                            if (rand(3) > 0 && worldIn->isAirBlock(position.add(0, l3, 1)))
                            {
                                addVine(worldIn, position.add(0, l3, 1), BlockVine::NORTH);
                            }
                        }
                    }
                }

                if (vinesGrow)
                {
                    for (auto l3 = position.gety() - 3 + i; l3 <= position.gety() + i; ++l3)
                    {
                        auto i4 = l3 - (position.gety() + i);
                        auto k4 = 2 - i4 / 2;
                        MutableBlockPos blockpos$mutableblockpos1;

                        for (auto l4 = position.getx() - k4; l4 <= position.getx() + k4; ++l4)
                        {
                            for (auto i5 = position.getz() - k4; i5 <= position.getz() + k4; ++i5)
                            {
                                blockpos$mutableblockpos1.setPos(l4, l3, i5);
                                if (worldIn->getBlockState(blockpos$mutableblockpos1)->getMaterial() ==
                                    Material::LEAVES)
                                {
                                    BlockPos blockpos2 = blockpos$mutableblockpos1.west();
                                    blockpos3          = blockpos$mutableblockpos1.east();
                                    BlockPos blockpos4 = blockpos$mutableblockpos1.north();
                                    BlockPos blockpos1 = blockpos$mutableblockpos1.south();
                                    if (rand(4) == 0 &&
                                        worldIn->getBlockState(blockpos2)->getMaterial() == Material::AIR)
                                    {
                                        addHangingVine(worldIn, blockpos2, BlockVine::EAST);
                                    }

                                    if (rand(4) == 0 &&
                                        worldIn->getBlockState(blockpos3)->getMaterial() == Material::AIR)
                                    {
                                        addHangingVine(worldIn, blockpos3, BlockVine::WEST);
                                    }

                                    if (rand(4) == 0 &&
                                        worldIn->getBlockState(blockpos4)->getMaterial() == Material::AIR)
                                    {
                                        addHangingVine(worldIn, blockpos4, BlockVine::SOUTH);
                                    }

                                    if (rand(4) == 0 &&
                                        worldIn->getBlockState(blockpos1)->getMaterial() == Material::AIR)
                                    {
                                        addHangingVine(worldIn, blockpos1, BlockVine::NORTH);
                                    }
                                }
                            }
                        }
                    }

                    if (rand(5) == 0 && i > 5)
                    {
                        for (auto l3 = 0; l3 < 2; ++l3)
                        {
                            for (auto enumfacing : Plane::HORIZONTAL)
                            {
                                if (rand(4 - l3) == 0)
                                {
                                    EnumFacing enumfacing1 = enumfacing.getOpposite();
                                    placeCocoa(
                                        worldIn, rand(3),
                                        position.add(enumfacing1.getXOffset(), i - 5 + l3, enumfacing1.getZOffset()),
                                        enumfacing);
                                }
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

void WorldGenTrees::placeCocoa(World *worldIn, int32_t p_181652_2_, const BlockPos &pos, EnumFacing side)
{
    setBlockAndNotifyAdequately(worldIn, pos,
                                Blocks::COCOA.getDefaultState()
                                    .withProperty(BlockCocoa::AGE, p_181652_2_)
                                    .withProperty(BlockCocoa::FACING, side));
}

void WorldGenTrees::addVine(World *worldIn, const BlockPos &pos, PropertyBool *prop)
{
    setBlockAndNotifyAdequately(worldIn, pos, Blocks::VINE.getDefaultState().withProperty(prop, true));
}

void WorldGenTrees::addHangingVine(World *worldIn, const BlockPos &pos, PropertyBool *prop)
{
    addVine(worldIn, pos, prop);
    auto i = 4;

    for (auto blockpos = pos.down(); worldIn->getBlockState(blockpos)->getMaterial() == Material::AIR && i > 0; --i)
    {
        addVine(worldIn, blockpos, prop);
        blockpos = blockpos.down();
    }
}
