#pragma once
#include "Item.h"

class ItemRedstone :public Item
{
public:
    ItemRedstone();
    EnumActionResult onItemUse(EntityPlayer* player, World* worldIn, BlockPos pos, EnumHand hand, EnumFacing facing, float hitX, float hitY, float hitZ) override;
};
