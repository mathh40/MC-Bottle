#include "ItemTippedArrow.h"
#include "ItemStack.h"
#include "../potion/PotionType.h"
#include "../potion/PotionUtils.h"
#include "text/translation/I18n.h"

ItemStack ItemTippedArrow::getDefaultInstance()
{
    return PotionUtils::addPotionToItemStack(ItemArrow::getDefaultInstance(), PotionTypes::POISON);
}

EntityArrow * ItemTippedArrow::createArrow(World *worldIn, ItemStack stack, EntityLivingBase *shooter)
{
    EntityTippedArrow* entitytippedarrow = new EntityTippedArrow(worldIn, shooter);
    entitytippedarrow->setPotionEffect(stack);
    return entitytippedarrow;
}

void ItemTippedArrow::getSubItems(const CreativeTabs& tab, std::vector<ItemStack>& items)
{
    if (isInCreativeTab(tab)) 
    {
        for(auto& potiontype : PotionType::REGISTRY)
        {
            if (!potiontype.second.getEffects().empty()) 
            {
                items.emplace_back(PotionUtils::addPotionToItemStack(ItemStack(this), potiontype.second));
            }
        }
    }
}

void ItemTippedArrow::addInformation(ItemStack stack, World* worldIn, std::vector<std::string>& tooltip, ITooltipFlag* flagIn)
{
    PotionUtils::addPotionTooltip(stack, tooltip, 0.125F);
}

std::string ItemTippedArrow::getItemStackDisplayName(ItemStack stack) const
{
    return I18n::translateToLocal(PotionUtils::getPotionFromItem(stack).getNamePrefixed("tipped_arrow.effect."));
}
