#include "WorldGenIceSpike.h"

#include "World.h"

bool WorldGenIceSpike::generate(World *worldIn, pcg32 &rand, const BlockPos &positionIn)
{
    BlockPos position;
    while (worldIn->isAirBlock(position) && position.gety() > 2)
    {
        position = positionIn.down();
    }

    if (worldIn->getBlockState(position).getBlock() != Blocks::SNOW)
    {
        return false;
    }
    else
    {
        position = position.up(rand(4));
        auto i   = rand(4) + 7;
        auto j   = i / 4 + rand(2);
        if (j > 1 && rand(60) == 0)
        {
            position = position.up(10 + rand(30));
        }

        for (auto k = 0; k < i; ++k)
        {
            float f = (1.0F - (float)k / (float)i) * (float)j;
            auto l  = MathHelper::ceil(f);

            for (auto i1 = -l; i1 <= l; ++i1)
            {
                auto f1 = (float)MathHelper::abs(i1) - 0.25F;

                for (auto j1 = -l; j1 <= l; ++j1)
                {
                    auto f2 = (float)MathHelper::abs(j1) - 0.25F;
                    if ((i1 == 0 && j1 == 0 || f1 * f1 + f2 * f2 <= f * f) &&
                        (i1 != -l && i1 != l && j1 != -l && j1 != l || MathHelper::nextFloat(rand) <= 0.75F))
                    {
                        auto iblockstate = worldIn->getBlockState(position.add(i1, k, j1));
                        auto block       = iblockstate->getBlock();
                        if (iblockstate->getMaterial() == Material::AIR || block == Blocks::DIRT ||
                            block == Blocks::SNOW || block == Blocks::ICE)
                        {
                            setBlockAndNotifyAdequately(worldIn, position.add(i1, k, j1),
                                                        Blocks::PACKED_ICE.getDefaultState());
                        }

                        if (k != 0 && l > 1)
                        {
                            iblockstate = worldIn->getBlockState(position.add(i1, -k, j1));
                            block       = iblockstate->getBlock();
                            if (iblockstate->getMaterial() == Material::AIR || block == Blocks::DIRT ||
                                block == Blocks::SNOW || block == Blocks::ICE)
                            {
                                setBlockAndNotifyAdequately(worldIn, position.add(i1, -k, j1),
                                                            Blocks::PACKED_ICE.getDefaultState());
                            }
                        }
                    }
                }
            }
        }

        auto k = j - 1;
        if (k < 0)
        {
            k = 0;
        }
        else if (k > 1)
        {
            k = 1;
        }

        for (auto l1 = -k; l1 <= k; ++l1)
        {
            for (auto l = -k; l <= k; ++l)
            {
                auto blockpos = position.add(l1, -1, l);
                auto j2       = 50;
                if (MathHelper::abs(l1) == 1 && MathHelper::abs(l) == 1)
                {
                    j2 = rand(5);
                }

                while (blockpos.gety() > 50)
                {
                    auto iblockstate1 = worldIn->getBlockState(blockpos);
                    auto block1       = iblockstate1->getBlock();
                    if (iblockstate1->getMaterial() != Material::AIR && block1 != Blocks::DIRT &&
                        block1 != Blocks::SNOW && block1 != Blocks::ICE && block1 != Blocks::PACKED_ICE)
                    {
                        break;
                    }

                    setBlockAndNotifyAdequately(worldIn, blockpos, Blocks::PACKED_ICE.getDefaultState());
                    blockpos = blockpos.down();
                    --j2;
                    if (j2 <= 0)
                    {
                        blockpos = blockpos.down(rand(5) + 1);
                        j2       = rand(5);
                    }
                }
            }
        }

        return true;
    }
}
