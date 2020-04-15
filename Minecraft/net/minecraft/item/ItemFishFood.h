#pragma once
#include "ItemFood.h"

class ItemFishFood :public ItemFood
{
public:
    ItemFishFood(bool cooked);
    int32_t getHealAmount(ItemStack stack) const override;
    float getSaturationModifier(ItemStack stack) const override;
    void getSubItems(const CreativeTabs& tab, std::vector<ItemStack>& items) override;
    std::string getTranslationKey(ItemStack stack) const override;
protected:
    void onFoodEaten(ItemStack stack, World* worldIn, EntityPlayer* player) const override;
private:
    bool cooked;
};