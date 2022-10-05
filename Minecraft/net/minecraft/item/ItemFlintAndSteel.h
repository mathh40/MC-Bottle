#pragma once
#include "Item.h"

class ItemFlintAndSteel : public Item
{
  public:
    ItemFlintAndSteel();
    EnumActionResult onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand, EnumFacing facing,
                               float hitX, float hitY, float hitZ) override;
};
