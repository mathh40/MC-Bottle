#include "WorldGenBlockBlob.h"

#include "World.h"

WorldGenBlockBlob::WorldGenBlockBlob(Block *blockIn, int32_t startRadiusIn)
    : WorldGenerator(false), block(blockIn), startRadius(startRadiusIn)
{
}

bool WorldGenBlockBlob::generate(World *worldIn, pcg32 &rand, const BlockPos &position)
{
    while (true)
    {
    label51 : {
        if (position.gety() > 3)
        {
            if (worldIn->isAirBlock(position.down()))
            {
                break label51;
            }

            auto block = worldIn->getBlockState(position.down()).getBlock();
            if (block != Blocks::GRASS && block != Blocks::DIRT && block != Blocks::STONE)
            {
                break label51;
            }
        }

        if (position.gety() <= 3)
        {
            return false;
        }

        auto i1 = startRadius;

        for (auto i = 0; i1 >= 0 && i < 3; ++i)
        {
            auto j = i1 + rand(2);
            auto k = i1 + rand(2);
            auto l = i1 + rand(2);
            auto f = (float)(j + k + l) * 0.333F + 0.5F;
            for (auto blockpos : getAllInBox(position.add(-j, -k, -l), position.add(j, k, l)))
            {
                if (blockpos.distanceSq(position) <= (double)(f * f))
                {
                    worldIn->setBlockState(blockpos, block->getDefaultState(), 4);
                }
            }

            position = position.add(-(i1 + 1) + rand(2 + i1 * 2), 0 - rand(2), -(i1 + 1) + rand(2 + i1 * 2));
        }

        return true;
    }

        position = position.down();
    }
}
