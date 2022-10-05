#include "WorldGenEndIsland.h"

#include "math/BlockPos.h"
#include "math/MathHelper.h"

bool WorldGenEndIsland::generate(World *worldIn, pcg32 &rand, const BlockPos &position)
{
    auto f = (float)(rand(3) + 4);

    for (auto i = 0; f > 0.5F; --i)
    {
        for (auto j = MathHelper::floor(-f); j <= MathHelper::ceil(f); ++j)
        {
            for (auto k = MathHelper::floor(-f); k <= MathHelper::ceil(f); ++k)
            {
                if ((float)(j * j + k * k) <= (f + 1.0F) * (f + 1.0F))
                {
                    setBlockAndNotifyAdequately(worldIn, position.add(j, i, k), Blocks::END_STONE.getDefaultState());
                }
            }
        }

        f = (float)((double)f - ((double)rand(2) + 0.5));
    }

    return true;
}
