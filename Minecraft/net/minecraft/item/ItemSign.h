#pragma once
#include "Item.h"

class ItemSign : public Item
{
    ItemSign();
    EnumActionResult onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand, EnumFacing facing,
                               float hitX, float hitY, float hitZ) override;
};
