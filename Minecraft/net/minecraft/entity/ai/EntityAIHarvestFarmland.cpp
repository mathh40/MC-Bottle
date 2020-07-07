#include "EntityAIHarvestFarmland.h"


#include "Block.h"
#include "Util.h"
#include "../../inventory/InventoryBasic.h"
#include "state/IBlockState.h"

EntityAIHarvestFarmland::EntityAIHarvestFarmland(EntityVillager *villagerIn, double speedIn)
    :EntityAIMoveToBlock(villagerIn, speedIn, 16),villager(villagerIn)
{
}

bool EntityAIHarvestFarmland::shouldExecute()
{
    if (runDelay <= 0) 
    {
        if (!villager->world.getGameRules().getBoolean("mobGriefing")) 
        {
            return false;
        }

        currentTask = -1;
        hasFarmItem = villager->isFarmItemInInventory();
        wantsToReapStuff = villager->wantsMoreFood();
    }

    return EntityAIMoveToBlock::shouldExecute();
}

bool EntityAIHarvestFarmland::shouldContinueExecuting()
{
    return currentTask >= 0 && EntityAIMoveToBlock::shouldContinueExecuting();
}

void EntityAIHarvestFarmland::updateTask()
{
    EntityAIMoveToBlock::updateTask();
    villager->getLookHelper().setLookPosition(destinationBlock.getx() + 0.5, destinationBlock.gety() + 1, destinationBlock.getz() + 0.5, 10.0F, villager->getVerticalFaceSpeed());
    if (getIsAboveDestination()) 
    {
        World* world = villager->world;
        BlockPos blockpos = destinationBlock.up();
        IBlockState* iblockstate = world->getBlockState(blockpos);
        Block* block = iblockstate->getBlock();
        if (currentTask == 0 && Util::instanceof<BlockCrops>(block) && ((BlockCrops*)block).isMaxAge(iblockstate))
        {
            world->destroyBlock(blockpos, true);
        }
        else if (currentTask == 1 && iblockstate->getMaterial() == Material::AIR) 
        {
            InventoryBasic* inventorybasic = villager->getVillagerInventory();

            for(auto i = 0; i < inventorybasic->getSizeInventory(); ++i) 
            {
                ItemStack itemstack = inventorybasic->getStackInSlot(i);
                bool flag = false;
                if (!itemstack.isEmpty()) 
                {
                    if (itemstack.getItem() == Items::WHEAT_SEEDS) 
                    {
                        world->setBlockState(blockpos, Blocks::WHEAT.getDefaultState(), 3);
                        flag = true;
                    }
                    else if (itemstack.getItem() == Items::POTATO) 
                    {
                        world->setBlockState(blockpos, Blocks::POTATOES.getDefaultState(), 3);
                        flag = true;
                    }
                    else if (itemstack.getItem() == Items::CARROT) 
                    {
                        world->setBlockState(blockpos, Blocks::CARROTS.getDefaultState(), 3);
                        flag = true;
                    }
                    else if (itemstack.getItem() == Items::BEETROOT_SEEDS) 
                    {
                        world->setBlockState(blockpos, Blocks::BEETROOTS.getDefaultState(), 3);
                        flag = true;
                    }
                }

                if (flag) {
                    itemstack.shrink(1);
                    if (itemstack.isEmpty()) 
                    {
                        inventorybasic->setInventorySlotContents(i, ItemStack::EMPTY);
                    }
                    break;
                }
            }
        }

        currentTask = -1;
        runDelay = 10;
    }
}

bool EntityAIHarvestFarmland::shouldMoveTo(World *worldIn, BlockPos pos)
{
    Block* block = worldIn->getBlockState(pos).getBlock();
    if (block == Blocks::FARMLAND) 
    {
        pos = pos.up();
        IBlockState* iblockstate = worldIn->getBlockState(pos);
        block = iblockstate->getBlock();
        if (Util::instanceof<BlockCrops>(block) && ((BlockCrops*)block).isMaxAge(iblockstate) && wantsToReapStuff && (currentTask == 0 || currentTask < 0)) 
        {
            currentTask = 0;
            return true;
        }

        if (iblockstate->getMaterial() == Material::AIR && hasFarmItem && (currentTask == 1 || currentTask < 0)) 
        {
            currentTask = 1;
            return true;
        }
    }

    return false;
}
