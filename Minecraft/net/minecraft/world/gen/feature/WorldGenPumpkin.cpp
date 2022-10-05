#include "WorldGenPumpkin.h"

#include "World.h"

bool WorldGenPumpkin::generate(World *worldIn, pcg32 &rand, const BlockPos &position)
{
    for (auto i = 0; i < 64; ++i)
    {
        auto blockpos = position.add((int32_t)(rand(8) - rand(8)), rand(4) - rand(4), rand(8) - rand(8));
        if (worldIn->isAirBlock(blockpos) && worldIn->getBlockState(blockpos.down()).getBlock() == Blocks::GRASS &&
            Blocks::PUMPKIN.canPlaceBlockAt(worldIn, blockpos))
        {
            worldIn->setBlockState(blockpos,
                                   Blocks::PUMPKIN.getDefaultState().withProperty(
                                       BlockPumpkin::FACING, EnumFacing::Plane::HORIZONTAL::random(rand)),
                                   2);
        }
    }

    return true;
}
