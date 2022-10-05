#pragma once
#include "ItemFood.h"

class ItemAppleGold : public ItemFood
{
  public:
    ItemAppleGold(int32_t amount, float saturation, bool isWolfFood);
    bool hasEffect(ItemStack stack) override;
    EnumRarity getRarity(ItemStack stack) override;
    void getSubItems(const CreativeTabs &tab, std::vector<ItemStack> &items) override;

  protected:
    void onFoodEaten(ItemStack stack, World *worldIn, EntityPlayer *player) const override;

  private:
};
