#pragma once
#include "Item.h"

class ItemSnowball : public Item
{
    ItemSnowball();
    ActionResult onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn) override;
};
