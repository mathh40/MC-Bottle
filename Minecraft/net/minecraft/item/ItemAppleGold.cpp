#include "ItemAppleGold.h"

#include "ItemStack.h"

ItemAppleGold::ItemAppleGold(int32_t amount, float saturation, bool isWolfFood)
    :ItemFood(amount, saturation, isWolfFood)
{
    setHasSubtypes(true);
}

bool ItemAppleGold::hasEffect(ItemStack stack)
{
    return ItemFood::hasEffect(stack) || stack.getMetadata() > 0;
}

EnumRarity ItemAppleGold::getRarity(ItemStack stack)
{
    return stack.getMetadata() == 0 ? EnumRarity::RARE : EnumRarity::EPIC;
}

void ItemAppleGold::onFoodEaten(ItemStack stack, World *worldIn, EntityPlayer *player)
{
    if (!worldIn->isRemote) 
    {
        if (stack.getMetadata() > 0) 
        {
            player->addPotionEffect(PotionEffect(MobEffects::REGENERATION, 400, 1));
            player->addPotionEffect(PotionEffect(MobEffects::RESISTANCE, 6000, 0));
            player->addPotionEffect(PotionEffect(MobEffects::FIRE_RESISTANCE, 6000, 0));
            player->addPotionEffect(PotionEffect(MobEffects::ABSORPTION, 2400, 3));
        }
        else 
        {
            player->addPotionEffect(PotionEffect(MobEffects::REGENERATION, 100, 1));
            player->addPotionEffect(PotionEffect(MobEffects::ABSORPTION, 2400, 0));
        }
    }
}

void ItemAppleGold::getSubItems(const CreativeTabs& tab, std::vector<ItemStack>& items)
{
    if (isInCreativeTab(tab)) 
    {
        items.push_back(ItemStack(this));
        items.push_back(ItemStack(this, 1, 1));
    }
}
