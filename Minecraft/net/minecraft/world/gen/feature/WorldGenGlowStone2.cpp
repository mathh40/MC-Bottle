#include "WorldGenGlowStone2.h"

#include "EnumFacing.h"
#include "World.h"
#include "material/Material.h"

bool WorldGenGlowStone2::generate(World *worldIn, pcg32 &rand, const BlockPos &position)
{
    if (!worldIn->isAirBlock(position))
    {
        return false;
    }
    else if (worldIn->getBlockState(position.up()).getBlock() != Blocks::NETHERRACK)
    {
        return false;
    }
    else
    {
        worldIn->setBlockState(position, Blocks::GLOWSTONE.getDefaultState(), 2);

        for (auto i = 0; i < 1500; ++i)
        {
            auto blockpos = position.add((int32_t)(rand(8) - rand(8)), -rand(12), rand(8) - rand(8));
            if (worldIn->getBlockState(blockpos).getMaterial() == Material::AIR)
            {
                auto j = 0;

                for (auto enumfacing : EnumFacing::values())
                {
                    if (worldIn->getBlockState(blockpos.offset(enumfacing)).getBlock() == Blocks::GLOWSTONE)
                    {
                        ++j;
                    }

                    if (j > 1)
                    {
                        break;
                    }
                }

                if (j == 1)
                {
                    worldIn->setBlockState(blockpos, Blocks::GLOWSTONE.getDefaultState(), 2);
                }
            }
        }

        return true;
    }
}
