#include "ItemLingeringPotion.h"

#include "../potion/PotionUtils.h"
#include "../stats/StatList.h"
#include "../world/World.h"
#include "../world/gen/ChunkGeneratorFlat.h"
#include "ItemStack.h"
#include "SoundCategory.h"
#include "text/translation/I18n.h"

std::string ItemLingeringPotion::getItemStackDisplayName(ItemStack stack) const
{
    return I18n::translateToLocal(PotionUtils::getPotionFromItem(stack).getNamePrefixed("lingering_potion.effect."));
}

void ItemLingeringPotion::addInformation(ItemStack stack, World *worldIn, std::vector<std::string> &tooltip,
                                         ITooltipFlag *flagIn)
{
    PotionUtils::addPotionTooltip(stack, tooltip, 0.25F);
}

ActionResult ItemLingeringPotion::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    ItemStack itemstack  = playerIn->getHeldItem(handIn);
    ItemStack itemstack1 = playerIn->capabilities.isCreativeMode ? itemstack.copy() : itemstack.splitStack(1);
    worldIn->playSound(nullptr, playerIn->posX, playerIn->posY, playerIn->posZ,
                       SoundEvents::ENTITY_LINGERINGPOTION_THROW, SoundCategory::NEUTRAL, 0.5F,
                       0.4F / (MathHelper::nextFloat(itemRand) * 0.4F + 0.8F));
    if (!worldIn->isRemote)
    {
        EntityPotion entitypotion = new EntityPotion(worldIn, playerIn, itemstack1);
        entitypotion.shoot(playerIn, playerIn->rotationPitch, playerIn->rotationYaw, -20.0F, 0.5F, 1.0F);
        worldIn->spawnEntity(entitypotion);
    }

    playerIn->addStat(StatList::getObjectUseStats(this));
    return ActionResult(EnumActionResult::SUCCESS, itemstack);
}
