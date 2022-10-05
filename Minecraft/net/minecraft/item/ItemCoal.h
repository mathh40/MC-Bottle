#pragma once
#include "Item.h"

class ItemCoal : public Item
{
  public:
    ItemCoal();
    std::string getTranslationKey(ItemStack stack) const override;
    void getSubItems(const CreativeTabs &tab, std::vector<ItemStack> &items);
};
