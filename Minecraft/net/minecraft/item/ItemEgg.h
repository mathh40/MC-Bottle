#pragma once
#include "Item.h"

class ItemEgg :public Item
{
public:
    ItemEgg();
    ActionResult onItemRightClick(World* worldIn, EntityPlayer* playerIn, EnumHand handIn) override;
};