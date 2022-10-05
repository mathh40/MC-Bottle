#include "WorldGeneratorBonusChest.h"

#include "World.h"
#include "material/Material.h"

bool WorldGeneratorBonusChest::generate(World *worldIn, pcg32 &rand, const BlockPos &position)
{
    for (auto iblockstate = worldIn->getBlockState(position);
         (iblockstate->getMaterial() == Material::AIR || iblockstate->getMaterial() == Material::LEAVES) &&
         position.gety() > 1;
         iblockstate = worldIn->getBlockState(position))
    {
        position = position.down();
    }

    if (position.gety() < 1)
    {
        return false;
    }
    else
    {
        position = position.up();

        for (auto i = 0; i < 4; ++i)
        {
            auto blockpos = position.add(rand(4) - rand(4), rand(3) - rand(3), rand(4) - rand(4));
            if (worldIn->isAirBlock(blockpos) && worldIn->getBlockState(blockpos.down()).isTopSolid())
            {
                worldIn->setBlockState(blockpos, Blocks.CHEST.getDefaultState(), 2);
                auto tileentity = worldIn->getTileEntity(blockpos);
                if (Util:: instanceof <TileEntityChest>(tileentity))
                {
                    ((TileEntityChest)tileentity)
                        ->setLootTable(LootTableList.CHESTS_SPAWN_BONUS_CHEST, MathHelper::nextLong(rand));
                }

                BlockPos blockpos1 = blockpos.east();
                BlockPos blockpos2 = blockpos.west();
                BlockPos blockpos3 = blockpos.north();
                BlockPos blockpos4 = blockpos.south();
                if (worldIn->isAirBlock(blockpos2) && worldIn->getBlockState(blockpos2.down()).isTopSolid())
                {
                    worldIn->setBlockState(blockpos2, Blocks.TORCH.getDefaultState(), 2);
                }

                if (worldIn->isAirBlock(blockpos1) && worldIn->getBlockState(blockpos1.down()).isTopSolid())
                {
                    worldIn->setBlockState(blockpos1, Blocks.TORCH.getDefaultState(), 2);
                }

                if (worldIn->isAirBlock(blockpos3) && worldIn->getBlockState(blockpos3.down()).isTopSolid())
                {
                    worldIn->setBlockState(blockpos3, Blocks.TORCH.getDefaultState(), 2);
                }

                if (worldIn->isAirBlock(blockpos4) && worldIn->getBlockState(blockpos4.down()).isTopSolid())
                {
                    worldIn->setBlockState(blockpos4, Blocks.TORCH.getDefaultState(), 2);
                }

                return true;
            }
        }

        return false;
    }
}
