#pragma once
#include "ItemBlock.h"

class ItemSnow : public ItemBlock
{
  public:
    ItemSnow(Block *block);
    EnumActionResult onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand, EnumFacing facing,
                               float hitX, float hitY, float hitZ) override;
    int32_t getMetadata(int32_t damage) const override;
};
