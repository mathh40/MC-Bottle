#include "WorldGenLiquids.h"

#include "World.h"

WorldGenLiquids::WorldGenLiquids(Block *blockIn) : block(blockIn)
{
}

bool WorldGenLiquids::generate(World *worldIn, pcg32 &rand, const BlockPos &position)
{
    if (worldIn->getBlockState(position.up())->getBlock() != Blocks::STONE)
    {
        return false;
    }
    else if (worldIn->getBlockState(position.down())->getBlock() != Blocks::STONE)
    {
        return false;
    }
    else
    {
        auto iblockstate = worldIn->getBlockState(position);
        if (iblockstate->getMaterial() != Material::AIR && iblockstate->getBlock() != Blocks::STONE)
        {
            return false;
        }
        else
        {
            auto i = 0;
            if (worldIn->getBlockState(position.west())->getBlock() == Blocks::STONE)
            {
                ++i;
            }

            if (worldIn->getBlockState(position.east())->getBlock() == Blocks::STONE)
            {
                ++i;
            }

            if (worldIn->getBlockState(position.north())->getBlock() == Blocks::STONE)
            {
                ++i;
            }

            if (worldIn->getBlockState(position.south())->getBlock() == Blocks::STONE)
            {
                ++i;
            }

            auto j = 0;
            if (worldIn->isAirBlock(position.west()))
            {
                ++j;
            }

            if (worldIn->isAirBlock(position.east()))
            {
                ++j;
            }

            if (worldIn->isAirBlock(position.north()))
            {
                ++j;
            }

            if (worldIn->isAirBlock(position.south()))
            {
                ++j;
            }

            if (i == 3 && j == 1)
            {
                auto iblockstate1 = block->getDefaultState();
                worldIn->setBlockState(position, iblockstate1, 2);
                worldIn->immediateBlockTick(position, iblockstate1, rand);
            }

            return true;
        }
    }
}
