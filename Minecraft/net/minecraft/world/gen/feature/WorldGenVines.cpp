#include "WorldGenVines.h"

#include "World.h"
#include "math/BlockPos.h"

bool WorldGenVines::generate(World *worldIn, pcg32 &rand, const BlockPos &positionIn)
{
    BlockPos position = positionIn;
    for (; position.gety() < 128; position = position.up())
    {
        if (worldIn->isAirBlock(position))
        {
            for (auto enumfacing : Plane::HORIZONTAL.facings())
            {
                if (Blocks::VINE.canPlaceBlockOnSide(worldIn, position, enumfacing))
                {
                    IBlockState *iblockstate = Blocks::VINE.getDefaultState()
                                                   .withProperty(BlockVine::NORTH, enumfacing == EnumFacing::NORTH)
                                                   .withProperty(BlockVine::EAST, enumfacing == EnumFacing::EAST)
                                                   .withProperty(BlockVine::SOUTH, enumfacing == EnumFacing::SOUTH)
                                                   .withProperty(BlockVine::WEST, enumfacing == EnumFacing::WEST);
                    worldIn->setBlockState(position, iblockstate, 2);
                    break;
                }
            }
        }
        else
        {
            position = position.add(rand.nextInt(4) - rand.nextInt(4), 0, rand.nextInt(4) - rand.nextInt(4));
        }
    }

    return true;
}
