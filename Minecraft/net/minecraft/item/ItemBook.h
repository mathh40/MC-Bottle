#pragma once
#include "Item.h"

class ItemBook : public Item
{
  public:
    bool isEnchantable(ItemStack stack) const override;
    int32_t getItemEnchantability() override;
};
