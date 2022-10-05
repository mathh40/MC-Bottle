#include "WorldGenDoublePlant.h"

#include "math/BlockPos.h"

void WorldGenDoublePlant::setPlantType(BlockDoublePlant::EnumPlantType plantTypeIn)
{
    plantType = plantTypeIn;
}

bool WorldGenDoublePlant::generate(World *worldIn, pcg32 &rand, const BlockPos &position)
{
    bool flag = false;

    for (auto i = 0; i < 64; ++i)
    {
        BlockPos blockpos = position.add((int32_t)(rand(8) - rand(8)), rand(4) - rand(4), rand(8) - rand(8));
        if (worldIn->isAirBlock(blockpos) && (!worldIn->provider.isNether() || blockpos.gety() < 254) &&
            Blocks::DOUBLE_PLANT.canPlaceBlockAt(worldIn, blockpos))
        {
            Blocks::DOUBLE_PLANT.placeAt(worldIn, blockpos, plantType, 2);
            flag = true;
        }
    }

    return flag;
}
