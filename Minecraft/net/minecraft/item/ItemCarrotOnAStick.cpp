#include "ItemCarrotOnAStick.h"

#include "../stats/StatList.h"
#include "ItemStack.h"
#include "Util.h"

ItemCarrotOnAStick::ItemCarrotOnAStick()
{
    setCreativeTab(CreativeTabs::TRANSPORTATION);
    setMaxStackSize(1);
    setMaxDamage(25);
}

bool ItemCarrotOnAStick::isFull3D() const
{
    return true;
}

bool ItemCarrotOnAStick::shouldRotateAroundWhenRendering()
{
    return true;
}

ActionResult ItemCarrotOnAStick::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    ItemStack itemstack = playerIn->getHeldItem(handIn);
    if (worldIn->isRemote)
    {
        return ActionResult(EnumActionResult::PASS, itemstack);
    }
    else
    {
        if (playerIn->isRiding() && Util:: instanceof <EntityPig>(playerIn->getRidingEntity()))
        {
            EntityPig *entitypig = (EntityPig *)playerIn->getRidingEntity();
            if (itemstack.getMaxDamage() - itemstack.getMetadata() >= 7 && entitypig->boost())
            {
                itemstack.damageItem(7, playerIn);
                if (itemstack.isEmpty())
                {
                    ItemStack itemstack1 = ItemStack(Items::FISHING_ROD);
                    itemstack1.setTagCompound(itemstack.getTagCompound());
                    return ActionResult(EnumActionResult::SUCCESS, itemstack1);
                }

                return ActionResult(EnumActionResult::SUCCESS, itemstack);
            }
        }

        playerIn->addStat(StatList::getObjectUseStats(this));
        return ActionResult(EnumActionResult::PASS, itemstack);
    }
}
