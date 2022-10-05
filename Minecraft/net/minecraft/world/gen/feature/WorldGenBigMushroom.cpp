#include "WorldGenBigMushroom.h"

#include "Util.h"
#include "spdlog/fmt/bundled/format.h"

WorldGenBigMushroom::WorldGenBigMushroom(Block *p_i46449_1_) : WorldGenerator(true), mushroomType(p_i46449_1_)
{
}

WorldGenBigMushroom::WorldGenBigMushroom() : WorldGenerator(false), mushroomType(nullptr)
{
}

bool WorldGenBigMushroom::generate(World *worldIn, pcg32 &rand, const BlockPos &position)
{
    auto block = mushroomType;
    if (block == nullptr)
    {
        Util::RandomizerWithSentinelShift shift;
        block = shift(rand) ? Blocks::BROWN_MUSHROOM_BLOCK : Blocks::RED_MUSHROOM_BLOCK;
    }

    auto i = rand(3) + 4;
    if (rand(12) == 0)
    {
        i *= 2;
    }

    bool flag = true;
    if (position.gety() >= 1 && position.gety() + i + 1 < 256)
    {
        for (auto j = position.gety(); j <= position.gety() + 1 + i; ++j)
        {
            auto k = 3;
            if (j <= position.gety() + 3)
            {
                k = 0;
            }

            MutableBlockPos blockpos$mutableblockpos;

            for (auto j3 = position.getx() - k; j3 <= position.getx() + k && flag; ++j3)
            {
                for (auto k3 = position.getz() - k; k3 <= position.getz() + k && flag; ++k3)
                {
                    if (j >= 0 && j < 256)
                    {
                        auto material =
                            worldIn->getBlockState(blockpos$mutableblockpos.setPos(j3, j, k3)).getMaterial();
                        if (material != Material::AIR && material != Material::LEAVES)
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
            auto block1 = worldIn->getBlockState(position.down()).getBlock();
            if (block1 != Blocks::DIRT && block1 != Blocks::GRASS && block1 != Blocks::MYCELIUM)
            {
                return false;
            }
            else
            {
                auto k2 = position.gety() + i;
                if (block == Blocks.RED_MUSHROOM_BLOCK)
                {
                    k2 = position.gety() + i - 3;
                }

                for (auto l2 = k2; l2 <= position.gety() + i; ++l2)
                {
                    auto j3 = 1;
                    if (l2 < position.gety() + i)
                    {
                        ++j3;
                    }

                    if (block == Blocks::BROWN_MUSHROOM_BLOCK)
                    {
                        j3 = 3;
                    }

                    auto k3 = position.getx() - j3;
                    auto l3 = position.getx() + j3;
                    auto j1 = position.getz() - j3;
                    auto k1 = position.getz() + j3;

                    for (int l1 = k3; l1 <= l3; ++l1)
                    {
                        for (int i2 = j1; i2 <= k1; ++i2)
                        {
                            int j2 = 5;
                            if (l1 == k3)
                            {
                                --j2;
                            }
                            else if (l1 == l3)
                            {
                                ++j2;
                            }

                            if (i2 == j1)
                            {
                                j2 -= 3;
                            }
                            else if (i2 == k1)
                            {
                                j2 += 3;
                            }

                            BlockHugeMushroom::EnumType blockhugemushroom$enumtype =
                                BlockHugeMushroom::EnumType.byMetadata(j2);
                            if (block == Blocks::BROWN_MUSHROOM_BLOCK || l2 < position.gety() + i)
                            {
                                if ((l1 == k3 || l1 == l3) && (i2 == j1 || i2 == k1))
                                {
                                    continue;
                                }

                                if (l1 == position.getx() - (j3 - 1) && i2 == j1)
                                {
                                    blockhugemushroom$enumtype = BlockHugeMushroom::EnumType.NORTH_WEST;
                                }

                                if (l1 == k3 && i2 == position.getz() - (j3 - 1))
                                {
                                    blockhugemushroom$enumtype = BlockHugeMushroom::EnumType.NORTH_WEST;
                                }

                                if (l1 == position.getx() + (j3 - 1) && i2 == j1)
                                {
                                    blockhugemushroom$enumtype = BlockHugeMushroom::EnumType.NORTH_EAST;
                                }

                                if (l1 == l3 && i2 == position.getz() - (j3 - 1))
                                {
                                    blockhugemushroom$enumtype = BlockHugeMushroom::EnumType.NORTH_EAST;
                                }

                                if (l1 == position.getx() - (j3 - 1) && i2 == k1)
                                {
                                    blockhugemushroom$enumtype = BlockHugeMushroom::EnumType.SOUTH_WEST;
                                }

                                if (l1 == k3 && i2 == position.getz() + (j3 - 1))
                                {
                                    blockhugemushroom$enumtype = BlockHugeMushroom::EnumType.SOUTH_WEST;
                                }

                                if (l1 == position.getx() + (j3 - 1) && i2 == k1)
                                {
                                    blockhugemushroom$enumtype = BlockHugeMushroom::EnumType.SOUTH_EAST;
                                }

                                if (l1 == l3 && i2 == position.getz() + (j3 - 1))
                                {
                                    blockhugemushroom$enumtype = BlockHugeMushroom::EnumType.SOUTH_EAST;
                                }
                            }

                            if (blockhugemushroom$enumtype == BlockHugeMushroom.EnumType.CENTER &&
                                l2 < position.gety() + i)
                            {
                                blockhugemushroom$enumtype = BlockHugeMushroom.EnumType.ALL_INSIDE;
                            }

                            if (position.gety() >= position.gety() + i - 1 ||
                                blockhugemushroom$enumtype != BlockHugeMushroom.EnumType.ALL_INSIDE)
                            {
                                BlockPos blockpos(l1, l2, i2);
                                if (!worldIn->getBlockState(blockpos).isFullBlock())
                                {
                                    setBlockAndNotifyAdequately(
                                        worldIn, blockpos,
                                        block->getDefaultState().withProperty(BlockHugeMushroom::VARIANT,
                                                                              blockhugemushroom$enumtype));
                                }
                            }
                        }
                    }
                }
            }

            for (auto l2 = 0; l2 < i; ++l2)
            {
                auto iblockstate = worldIn->getBlockState(position.up(l2));
                if (!iblockstate.isFullBlock())
                {
                    setBlockAndNotifyAdequately(worldIn, position.up(l2),
                                                block->getDefaultState().withProperty(
                                                    BlockHugeMushroom::VARIANT, BlockHugeMushroom::EnumType::STEM));
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
