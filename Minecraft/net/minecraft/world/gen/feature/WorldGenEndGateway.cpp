#include "WorldGenEndGateway.h"

#include "math/BlockPos.h"
#include "math/MathHelper.h"

bool WorldGenEndGateway::generate(World *worldIn, pcg32 &rand, const BlockPos &position)
{
    while (true)
    {
        for (auto blockpos$mutableblockpos : getAllInBoxMutable(position.add(-1, -2, -1), position.add(1, 2, 1)))
        {
            auto flag  = blockpos$mutableblockpos.getx() == position.getx();
            auto flag1 = blockpos$mutableblockpos.gety() == position.gety();
            auto flag2 = blockpos$mutableblockpos.getz() == position.getz();
            auto flag3 = MathHelper::abs(blockpos$mutableblockpos.gety() - position.gety()) == 2;
            if (flag && flag1 && flag2)
            {
                setBlockAndNotifyAdequately(worldIn, blockpos$mutableblockpos, Blocks::END_GATEWAY.getDefaultState());
            }
            else if (flag1)
            {
                setBlockAndNotifyAdequately(worldIn, blockpos$mutableblockpos, Blocks::AIR.getDefaultState());
            }
            else if (flag3 && flag && flag2)
            {
                setBlockAndNotifyAdequately(worldIn, blockpos$mutableblockpos, Blocks::BEDROCK.getDefaultState());
            }
            else if ((flag || flag2) && !flag3)
            {
                setBlockAndNotifyAdequately(worldIn, blockpos$mutableblockpos, Blocks::BEDROCK.getDefaultState());
            }
            else
            {
                setBlockAndNotifyAdequately(worldIn, blockpos$mutableblockpos, Blocks::AIR.getDefaultState());
            }
        }

        return true;
    }
}
