#include "WorldGenReed.h"

#include "World.h"
#include "math/BlockPos.h"

bool WorldGenReed::generate(World *worldIn, pcg32 &rand, const BlockPos &position)
{
    for (auto i = 0; i < 20; ++i)
    {
        auto blockpos = position.add((int32_t)(rand(4) - rand(4)), 0, rand(4) - rand(4));
        if (worldIn->isAirBlock(blockpos))
        {
            auto blockpos1 = blockpos.down();
            if (worldIn->getBlockState(blockpos1.west())->getMaterial() == Material::WATER ||
                worldIn->getBlockState(blockpos1.east()).getMaterial() == Material::WATER ||
                worldIn->getBlockState(blockpos1.north()).getMaterial() == Material::WATER ||
                worldIn->getBlockState(blockpos1.south()).getMaterial() == Material::WATER)
            {
                auto j = 2 + rand(rand(3) + 1);

                for (auto k = 0; k < j; ++k)
                {
                    if (Blocks::REEDS.canBlockStay(worldIn, blockpos))
                    {
                        worldIn->setBlockState(blockpos.up(k), Blocks::REEDS.getDefaultState(), 2);
                    }
                }
            }
        }
    }

    return true;
}
