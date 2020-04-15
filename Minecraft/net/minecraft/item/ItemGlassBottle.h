#pragma once
#include "Item.h"

class ItemGlassBottle :public Item
{
public:
    ItemGlassBottle();
    ActionResult onItemRightClick(World* worldIn, EntityPlayer* playerIn, EnumHand handIn) override;
protected:
    ItemStack turnBottleIntoItem(ItemStack p_185061_1_, EntityPlayer* player, ItemStack stack);
};