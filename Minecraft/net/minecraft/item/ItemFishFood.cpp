#include"ItemFishFood.h"


#include "EnumFishType.h"
#include "ItemStack.h"

ItemFishFood::ItemFishFood(bool cooked)
    :ItemFood(0,0.0f,false),cooked(cooked)
{
}

int32_t ItemFishFood::getHealAmount(ItemStack stack) const
{
    const FishType itemfishfood$fishtype = FishType::byItemStack(stack);
    return cooked && itemfishfood$fishtype.canCook() ? itemfishfood$fishtype.getCookedHealAmount() : itemfishfood$fishtype.getUncookedHealAmount();
}

float ItemFishFood::getSaturationModifier(ItemStack stack) const
{
    const FishType itemfishfood$fishtype = FishType::byItemStack(stack);
    return cooked && itemfishfood$fishtype.canCook() ? itemfishfood$fishtype.getCookedSaturationModifier() : itemfishfood$fishtype.getUncookedSaturationModifier();
}

void ItemFishFood::onFoodEaten(ItemStack stack, World *worldIn, EntityPlayer *player) const
{
    FishType itemfishfood$fishtype = FishType::byItemStack(stack);
    if (itemfishfood$fishtype == FishType::PUFFERFISH) 
    {
        player->addPotionEffect(PotionEffect(MobEffects::POISON, 1200, 3));
        player->addPotionEffect(PotionEffect(MobEffects::HUNGER, 300, 2));
        player->addPotionEffect(PotionEffect(MobEffects::NAUSEA, 300, 1));
    }

    ItemFood::onFoodEaten(stack, worldIn, player);
}

void ItemFishFood::getSubItems(const CreativeTabs& tab, std::vector<ItemStack>& items)
{
    if (isInCreativeTab(tab)) 
    {
        FishType[] var3 = FishType::values();
        int var4 = var3.length;

        for(int var5 = 0; var5 < var4; ++var5) 
        {
            FishType itemfishfood$fishtype = var3[var5];
            if (!cooked || itemfishfood$fishtype.canCook()) 
            {
                items.push_back(ItemStack(this, 1, itemfishfood$fishtype.getMetadata()));
            }
        }
    }
}

std::string ItemFishFood::getTranslationKey(ItemStack stack) const
{
    FishType itemfishfood$fishtype = FishType::byItemStack(stack);
    return getTranslationKey() + "." + itemfishfood$fishtype.getTranslationKey() + "." + (cooked && itemfishfood$fishtype.canCook() ? "cooked" : "raw");
}
