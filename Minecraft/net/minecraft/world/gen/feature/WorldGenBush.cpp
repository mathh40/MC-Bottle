#include "WorldGenBush.h"

#include "World.h"

WorldGenBush::WorldGenBush(BlockBush blockIn) : block(blockIn)
{
}

bool WorldGenBush::generate(World *worldIn, pcg32 &rand, const BlockPos &position)
{
    for (auto i = 0; i < 64; ++i)
    {
        auto blockpos = position.add(rand(8) - rand(8), rand(4) - rand(4), rand(8) - rand(8));
        if (worldIn->isAirBlock(blockpos) && (!worldIn->provider.isNether() || blockpos.getY() < 255) &&
            block.canBlockStay(worldIn, blockpos, block.getDefaultState()))
        {
            worldIn->setBlockState(blockpos, block.getDefaultState(), 2);
        }
    }

    return true;
}
