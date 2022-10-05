#include "ItemEgg.h"

#include "../stats/StatList.h"
#include "ItemStack.h"
#include "SoundCategory.h"

ItemEgg::ItemEgg()
{
    maxStackSize = 16;
    setCreativeTab(CreativeTabs::MATERIALS);
}

ActionResult ItemEgg::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    ItemStack itemstack = playerIn->getHeldItem(handIn);
    if (!playerIn->capabilities.isCreativeMode)
    {
        itemstack.shrink(1);
    }

    worldIn->playSound((EntityPlayer *)nullptr, playerIn->posX, playerIn->posY, playerIn->posZ,
                       SoundEvents::ENTITY_EGG_THROW, SoundCategory::PLAYERS, 0.5F,
                       0.4F / (itemRand.nextFloat() * 0.4F + 0.8F));
    if (!worldIn->isRemote)
    {
        EntityEgg *entityegg = EntityEgg(worldIn, playerIn);
        entityegg->shoot(playerIn, playerIn->rotationPitch, playerIn->rotationYaw, 0.0F, 1.5F, 1.0F);
        worldIn->spawnEntity(entityegg);
    }

    playerIn->addStat(StatList::getObjectUseStats(this));
    return ActionResult(EnumActionResult::SUCCESS, itemstack);
}
