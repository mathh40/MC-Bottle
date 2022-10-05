#pragma once
#include "Item.h"

class ItemElytra : public Item
{
  public:
    ItemElytra();
    static bool isUsable(ItemStack stack);
    bool getIsRepairable(ItemStack toRepair, ItemStack repair) override;
    ActionResult onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn) override;
};
