#include "ItemSplashPotion.h"

#include "../potion/PotionUtils.h"
#include "../stats/StatList.h"
#include "ItemStack.h"
#include "SoundCategory.h"
#include "biome/Biome.h"
#include "text/translation/I18n.h"

std::string ItemSplashPotion::getItemStackDisplayName(ItemStack stack) const
{
    return I18n::translateToLocal(PotionUtils::getPotionFromItem(stack).getNamePrefixed("splash_potion.effect."));
}

ActionResult ItemSplashPotion::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    ItemStack itemstack  = playerIn->getHeldItem(handIn);
    ItemStack itemstack1 = playerIn->capabilities.isCreativeMode ? itemstack.copy() : itemstack.splitStack(1);
    worldIn->playSound(nullptr, playerIn->posX, playerIn->posY, playerIn->posZ, SoundEvents.ENTITY_SPLASH_POTION_THROW,
                       SoundCategory::PLAYERS, 0.5F, 0.4F / (MathHelper::nextFloat(itemRand) * 0.4F + 0.8F));
    if (!worldIn->isRemote)
    {
        EntityPotion *entitypotion = new EntityPotion(worldIn, playerIn, itemstack1);
        entitypotion->shoot(playerIn, playerIn->rotationPitch, playerIn->rotationYaw, -20.0F, 0.5F, 1.0F);
        worldIn->spawnEntity(entitypotion);
    }

    playerIn->addStat(StatList::getObjectUseStats(this));
    return ActionResult(EnumActionResult::SUCCESS, itemstack);
}
