#pragma once
#include "Item.h"

class ItemDoor : public Item
{
  public:
    ItemDoor(Block *block);
    EnumActionResult onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand, EnumFacing facing,
                               float hitX, float hitY, float hitZ) override;
    void placeDoor(World *worldIn, BlockPos pos, EnumFacing facing, Block *door, bool isRightHinge);

  private:
    Block *block;
};
