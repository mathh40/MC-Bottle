#include "ItemEmptyMap.h"

#include "../stats/StatList.h"
#include "ItemStack.h"

ActionResult ItemEmptyMap::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    ItemStack itemstack  = ItemMap::setupNewMap(worldIn, playerIn->posX, playerIn->posZ, 0, true, false);
    ItemStack itemstack1 = playerIn->getHeldItem(handIn);
    itemstack1.shrink(1);
    if (itemstack1.isEmpty())
    {
        return ActionResult(EnumActionResult::SUCCESS, itemstack);
    }
    else
    {
        if (!playerIn->inventory.addItemStackToInventory(itemstack.copy()))
        {
            playerIn->dropItem(itemstack, false);
        }

        playerIn->addStat(StatList::getObjectUseStats(this));
        return ActionResult(EnumActionResult::SUCCESS, itemstack1);
    }
}

ItemEmptyMap::ItemEmptyMap()
{
    setCreativeTab(CreativeTabs::MISC);
}
