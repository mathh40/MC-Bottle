#include "WorldGenDeadBush.h"

#include "material/Material.h"
#include "math/BlockPos.h"

bool WorldGenDeadBush::generate(World *worldIn, pcg32 &rand, const BlockPos &position)
{
    for (IBlockState *iblockstate = worldIn->getBlockState(position);
         (iblockstate->getMaterial() == Material::AIR || iblockstate.getMaterial() == Material::LEAVES) &&
         position.gety() > 0;
         iblockstate = worldIn.getBlockState(position))
    {
        position = position.down();
    }

    for (auto i = 0; i < 4; ++i)
    {
        auto blockpos = position.add(rand(8) - rand(8), rand(4) - rand(4), rand(8) - rand(8));
        if (worldIn->isAirBlock(blockpos) &&
            Blocks.DEADBUSH.canBlockStay(worldIn, blockpos, Blocks.DEADBUSH.getDefaultState()))
        {
            worldIn->setBlockState(blockpos, Blocks.DEADBUSH.getDefaultState(), 2);
        }
    }

    return true;
}
