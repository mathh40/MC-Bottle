#pragma once
#include "ItemColored.h"

class ItemLilyPad : public ItemColored
{
  public:
    ItemLilyPad(Block *block);
    ActionResult onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn) override;
};
