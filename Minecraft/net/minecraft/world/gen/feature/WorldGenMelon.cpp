#include "WorldGenMelon.h"

#include "World.h"

bool WorldGenMelon::generate(World *worldIn, pcg32 &rand, const BlockPos &position)
{
    for (auto i = 0; i < 64; ++i)
    {
        auto blockpos = position.add((int32_t)(rand(8) - rand(8)), rand(4) - rand(4), rand(8) - rand(8));
        if (Blocks::MELON_BLOCK.canPlaceBlockAt(worldIn, blockpos) &&
            worldIn->getBlockState(blockpos.down()).getBlock() == Blocks::GRASS)
        {
            worldIn->setBlockState(blockpos, Blocks::MELON_BLOCK.getDefaultState(), 2);
        }
    }

    return true;
}
