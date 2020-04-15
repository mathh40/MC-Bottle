#include "ItemBucketMilk.h"


#include "ItemStack.h"
#include "Util.h"
#include "../stats/StatList.h"

ItemBucketMilk::ItemBucketMilk()
{
    setMaxStackSize(1);
    setCreativeTab(CreativeTabs::MISC);
}

ItemStack ItemBucketMilk::onItemUseFinish(ItemStack stack, World *worldIn, EntityLivingBase *entityLiving)
{
    if (Util::instanceof<EntityPlayerMP>(entityLiving)) 
    {
        EntityPlayerMP* entityplayermp = (EntityPlayerMP*)entityLiving;
        CriteriaTriggers::CONSUME_ITEM::trigger(entityplayermp, stack);
        entityplayermp->addStat(StatList::getObjectUseStats(this));
    }

    if (Util::instanceof<EntityPlayer>(entityLiving) && !((EntityPlayer*)entityLiving)->capabilities.isCreativeMode) 
    {
        stack.shrink(1);
    }

    if (!worldIn->isRemote) 
    {
        entityLiving->clearActivePotions();
    }

    return stack.isEmpty() ? ItemStack(Items::BUCKET) : stack;
}

int32_t ItemBucketMilk::getMaxItemUseDuration(ItemStack stack)
{
    return 32;
}

ActionResult ItemBucketMilk::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    playerIn->setActiveHand(handIn);
    return ActionResult(EnumActionResult::SUCCESS, playerIn->getHeldItem(handIn));
}

::EnumAction ItemBucketMilk::getItemUseAction(ItemStack stack)
{
    return EnumAction::DRINK;
}
