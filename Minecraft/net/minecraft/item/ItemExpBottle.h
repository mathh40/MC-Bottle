#pragma once
#include "Item.h"

class ItemExpBottle :public Item
{
public:
    ItemExpBottle();
    bool hasEffect(ItemStack stack) override;
    ActionResult onItemRightClick(World* worldIn, EntityPlayer* playerIn, EnumHand handIn) override;
};