#include "WorldGenCactus.h"

bool WorldGenCactus::generate(World *worldIn, pcg32 &rand, const BlockPos &position)
{
    for (auto i = 0; i < 10; ++i)
    {
        auto blockpos = position.add(rand(8) - rand(8), rand(4) - rand(4), rand(8) - rand(8));
        if (worldIn->isAirBlock(blockpos))
        {
            auto j = 1 + rand(rand(3) + 1);

            for (auto k = 0; k < j; ++k)
            {
                if (Blocks::CACTUS.canBlockStay(worldIn, blockpos))
                {
                    worldIn->setBlockState(blockpos.up(k), Blocks::CACTUS.getDefaultState(), 2);
                }
            }
        }
    }

    return true;
}
