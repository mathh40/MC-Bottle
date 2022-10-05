#include "WorldGenerator.h"

WorldGenerator::WorldGenerator() : doBlockNotify(false)
{
}

WorldGenerator::WorldGenerator(bool notify) : doBlockNotify(notify)
{
}

void WorldGenerator::setDecorationDefaults()
{
}

void WorldGenerator::setBlockAndNotifyAdequately(World *worldIn, const BlockPos &pos, IBlockState *state)
{
    if (doBlockNotify)
    {
        worldIn->setBlockState(pos, state, 3);
    }
    else
    {
        worldIn->setBlockState(pos, state, 2);
    }
}
