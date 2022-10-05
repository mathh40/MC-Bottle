#pragma once
#include "Item.h"

class ItemSimpleFoiled : public Item
{
  public:
    bool hasEffect(ItemStack stack) override;
};
