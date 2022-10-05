#pragma once
#include "ItemMapBase.h"

class ItemEmptyMap : public ItemMapBase
{
  public:
    ActionResult onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn) override;

  protected:
    ItemEmptyMap();
};
