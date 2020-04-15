#pragma once
#include "Item.h"

class ItemFireball :public Item
{
public:
    ItemFireball();
    EnumActionResult onItemUse(EntityPlayer* player, World* worldIn, BlockPos pos, EnumHand hand, EnumFacing facing, float hitX, float hitY, float hitZ) override;
};