#pragma once
#include "Item.h"
#include "NBTTagList.h"

class ItemEnchantedBook :public Item
{
public:
    bool hasEffect(ItemStack stack) override;
    bool isEnchantable(ItemStack stack) const override;
    EnumRarity getRarity(ItemStack stack) override;
    NBTTagList* getEnchantments(ItemStack p_92110_0_);
    void addInformation(ItemStack stack, World* worldIn, std::vector<std::string>& tooltip, ITooltipFlag* flagIn) override;
    void addEnchantment(ItemStack p_92115_0_, EnchantmentData stack);
    ItemStack getEnchantedItemStack(EnchantmentData p_92111_0_);
    void getSubItems(const CreativeTabs& tab, std::vector<ItemStack>& items);


private:
};
