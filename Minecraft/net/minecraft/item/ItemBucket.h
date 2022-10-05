#pragma once
#include "Item.h"

class ItemBucket : public Item
{
  public:
    ItemBucket(Block *containedBlockIn);
    ActionResult onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn) override;
    bool tryPlaceContainedLiquid(EntityPlayer *player, World *worldIn, BlockPos posIn);

  private:
    ItemStack fillBucket(ItemStack emptyBuckets, EntityPlayer *player, Item *fullBucket);

    Block *containedBlock;
};
