#pragma once
#include "Item.h"

class ItemLead :public Item
{
public:
    ItemLead();
    EnumActionResult onItemUse(EntityPlayer* player, World* worldIn, BlockPos pos, EnumHand hand, EnumFacing facing, float hitX, float hitY, float hitZ) override;
    static bool attachToFence(EntityPlayer* player, World* worldIn, BlockPos fence);
};