#pragma once
#include "Item.h"

class ItemEnderPearl :public Item
{
public:
    ItemEnderPearl();
    ActionResult onItemRightClick(World* worldIn, EntityPlayer* playerIn, EnumHand handIn) override;
};